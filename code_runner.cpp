#include "code_runner.h"
#include "language_registry.h"
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>
#include <QDateTime>
#include <QElapsedTimer>
#include <QCoreApplication>
#include <QDebug>
#include "output_normalizer.h"
#include "driver_generator.h"

CodeRunner::CodeRunner(LanguageRegistry *registry, QObject *parent)
    : QObject(parent), m_registry(registry) {

    m_pythonRunner = new PythonRunner();
    m_wasmRunnerC = new WasmRunner(false);
    m_wasmRunnerCpp = new WasmRunner(true);
    m_javascriptRunner = new JavascriptRunner();
}

CodeRunner::~CodeRunner() {
    delete m_pythonRunner;
    delete m_wasmRunnerC;
    delete m_wasmRunnerCpp;
    delete m_javascriptRunner;
}

EmbeddedRunner* CodeRunner::getRunner(const QString &languageId) {
    if (languageId == "python") return m_pythonRunner;
    if (languageId == "c") return m_wasmRunnerC;
    if (languageId == "cpp" || languageId == "c++") return m_wasmRunnerCpp;
    if (languageId == "javascript" || languageId == "js") return m_javascriptRunner;
    return nullptr;
}

void CodeRunner::runCode(const QString &code, const QString &languageId, const QString &problemPath) {
    if (m_running) {
        emit systemError("Already running");
        return;
    }

    m_running = true;
    m_stopRequested = false;
    emit started();

    QJsonArray tests;
    MethodSchema schema;
    if (!loadTestCases(problemPath, tests, schema)) {
        emit systemError("Failed to load test cases from: " + problemPath);
        m_running = false;
        emit finished();
        return;
    }

    qDebug() << "Loaded" << tests.size() << "test cases";

    EmbeddedRunner *runner = getRunner(languageId);
    if (!runner) {
        QString msg = QStringLiteral("Language not supported: ") + languageId;
        emit systemError(msg);
        m_running = false;
        emit finished();
        return;
    }
    
    if (languageId == "python" && !m_currentManifest.isEmpty()) {
        runPythonTestsBatch(code, m_currentManifest, -1);
    } else if ((languageId == "cpp" || languageId == "c++") && !m_currentManifest.isEmpty()) {
        runCppTestsBatch(code, m_currentManifest, -1);
    } else if ((languageId == "javascript" || languageId == "js") && !m_currentManifest.isEmpty()) {
        runJavascriptTestsBatch(code, m_currentManifest, -1);
    } else {
        QString fullCode = code;
        int offset = 0;
        if (schema.isValid()) {
            QString header = DriverGenerator::generateHeader(schema, languageId);
            offset = header.count('\n');
            fullCode = header + fullCode + DriverGenerator::generateDriver(schema, languageId);
        }

        for (int i = 0; i < tests.size() && !m_stopRequested; ++i) {
            emit progress(i + 1, tests.size());
            executeTestRaw(runner, fullCode, tests[i].toObject(), i, offset);
        }
    }

    m_running = false;
    emit finished();
}

void CodeRunner::runSingleTest(const QString &code, const QString &languageId,
                               int testIndex, const QString &problemPath) {
    if (m_running) {
        emit systemError("Already running");
        return;
    }

    m_running = true;
    m_stopRequested = false;
    emit started();

    QJsonArray tests;
    MethodSchema schema;
    if (!loadTestCases(problemPath, tests, schema)) {
        qDebug() << "Failed to load test cases from:" << problemPath;
        emit systemError("Failed to load test cases from: " + problemPath);
        m_running = false;
        emit finished();
        return;
    }

    if (testIndex < 0 || testIndex >= tests.size()) {
        qDebug() << "Test index" << testIndex << "out of range. Total tests:" << tests.size();
        emit systemError("Test index " + QString::number(testIndex) +
                         " out of range (0-" + QString::number(tests.size() - 1) + ")");
        m_running = false;
        emit finished();
        return;
    }

    EmbeddedRunner *runner = getRunner(languageId);
    if (!runner) {
        QString msg = QStringLiteral("Language not supported: ") + languageId;
        emit systemError(msg);
        m_running = false;
        emit finished();
        return;
    }

    if (languageId == "python" && !m_currentManifest.isEmpty()) {
        runPythonTestsBatch(code, m_currentManifest, testIndex);
    } else if ((languageId == "cpp" || languageId == "c++") && !m_currentManifest.isEmpty()) {
        runCppTestsBatch(code, m_currentManifest, testIndex);
    } else if ((languageId == "javascript" || languageId == "js") && !m_currentManifest.isEmpty()) {
        runJavascriptTestsBatch(code, m_currentManifest, testIndex);
    } else {
        QString fullCode = code;
        int offset = 0;
        if (schema.isValid()) {
            QString header = DriverGenerator::generateHeader(schema, languageId);
            offset = header.count('\n');
            fullCode = header + fullCode + DriverGenerator::generateDriver(schema, languageId);
        }

        executeTestRaw(runner, fullCode, tests[testIndex].toObject(), testIndex, offset);
    }

    m_running = false;
    emit finished();
}







void CodeRunner::runFreeCode(const QString &code, const QString &languageId) {
    // BUG-09: guard against concurrent execution (same guard as runCode/runSingleTest)
    if (m_running) {
        emit systemError("Already running");
        return;
    }
    m_running = true;
    m_stopRequested = false;

    EmbeddedRunner *runner = getRunner(languageId);
    if (!runner) {
        QString msg = QStringLiteral("Language not supported: ") + languageId;
        emit systemError(msg);
        m_running = false;
        emit finished();
        return;
    }

    QElapsedTimer timer;
    timer.start();
    EmbeddedRunner::Result r = runner->execute(code, "", &m_stopRequested);
    qint64 ms = timer.elapsed();

    if (!r.output.isEmpty()) emit freeCodeOutput(r.output);
    if (!r.error.isEmpty())  emit freeCodeError(r.error);
    emit freeCodeFinished(r.exitCode, ms);

    m_running = false;
    emit finished();
}

void CodeRunner::stop() {
    m_stopRequested = true;
}

bool CodeRunner::loadTestCases(const QString &problemPath, QJsonArray &tests, MethodSchema &schema) {
    qDebug() << "Loading test cases from:" << problemPath;
    
    auto parseDoc = [&](const QJsonDocument& doc) {
        tests = doc.object()["testCases"].toArray();
        if (doc.object().contains("method")) {
            schema = MethodSchema::fromJson(doc.object()["method"].toObject());
        }
        if (doc.object().contains("manifest")) {
            m_currentManifest = doc.object()["manifest"].toObject();
        } else {
            m_currentManifest = QJsonObject();
        }
        return !tests.isEmpty();
    };

    // Try the path directly first (it might be a full path)
    if (QFile::exists(problemPath)) {
        QFile file(problemPath);
        if (file.open(QIODevice::ReadOnly)) {
            QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
            file.close();
            bool hasTests = parseDoc(doc);
            qDebug() << "Loaded" << tests.size() << "test cases from:" << problemPath;
            return hasTests;
        }
    }

    QString resolvedPath = getProblemsPath(problemPath);
    if (!resolvedPath.isEmpty()) {
        QFile file(resolvedPath);
        if (file.open(QIODevice::ReadOnly)) {
            QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
            file.close();
            bool hasTests = parseDoc(doc);
            qDebug() << "Loaded" << tests.size() << "test cases from:" << resolvedPath;
            return hasTests;
        }
    }

    qDebug() << "Could not find problem file for:" << problemPath;
    return false;
}

QString CodeRunner::getProblemsPath(const QString &problemId) const {
    // Try multiple path patterns
    QStringList relPaths = {
        "/data/problems/" + problemId + ".json",
        "/problems/" + problemId + ".json",
        "/problems/easy/" + problemId + ".json",
        "/problems/medium/" + problemId + ".json",
        "/problems/hard/" + problemId + ".json",
        problemId + ".json",
        problemId
    };

    QStringList basePaths = {
        QDir::currentPath(),
        QCoreApplication::applicationDirPath(),
        QCoreApplication::applicationDirPath() + "/..",
        QCoreApplication::applicationDirPath() + "/../..",
        QCoreApplication::applicationDirPath() + "/../../.."
    };

    for (const QString &base : basePaths) {
        for (const QString &rel : relPaths) {
            QString full = QDir(base).cleanPath(base + rel);
            if (QFile::exists(full)) {
                qDebug() << "Found problem file at:" << full;
                return full;
            }
        }
    }

    return "";
}


void CodeRunner::executeTestRaw(EmbeddedRunner* runner, const QString &code,
                                const QJsonObject &test,
                                int index, int offset)
{
    QString input    = test["input"].toString();
    QString expected = test["expected"].toString().trimmed();

    QElapsedTimer timer;
    timer.start();
    
    EmbeddedRunner::Result r = runner->execute(code, input, &m_stopRequested);
    
    qint64 elapsedMs = timer.elapsed();

    QString actual = r.output.trimmed();
    QString status;

    if (r.exitCode != 0 && actual.isEmpty()) {
        status = "Runtime Error";
        actual = OutputNormalizer::normalizeError(r.error, runner->languageId(), offset).trimmed();
        if (actual.isEmpty()) {
            actual = QString("Process exited with code %1").arg(r.exitCode);
            if (!r.error.isEmpty()) {
                actual += "\nError: " + OutputNormalizer::normalizeError(r.error, runner->languageId(), offset);
            } else if (r.exitCode == -1 && runner->languageId() == "c") {
                actual += "\n(Check if 'main' function is defined, or if there was a relocation error)";
            }
        }
    } else if (r.timedOut) {
        status = "Time Limit Exceeded";
    } else {
        if (r.exitCode != 0 && actual.isEmpty() && !r.error.isEmpty()) {
            emit compilationError(OutputNormalizer::normalizeError(r.error, runner->languageId(), offset));
            return;
        }

        if (r.exitCode != 0) {
            status = "Runtime Error";
        } else {
            status = OutputNormalizer::equals(actual, expected) ? "Accepted" : "Wrong Answer";
        }
    }

    emit testResult(index, status, actual, expected, elapsedMs);
}

void CodeRunner::runPythonTestsBatch(const QString &code, const QJsonObject &manifest, int singleTestIndex) {
    QJsonObject runManifest = manifest;
    
    // If running single test, filter tests array
    if (singleTestIndex >= 0) {
        QJsonArray allTests = manifest["tests"].toArray();
        if (singleTestIndex < allTests.size()) {
            QJsonArray singleTestArray;
            singleTestArray.append(allTests[singleTestIndex]);
            runManifest["tests"] = singleTestArray;
        }
    }
    
    QJsonDocument manifestDoc(runManifest);
    QString manifestJson = QString::fromUtf8(manifestDoc.toJson(QJsonDocument::Compact));
    
    QString harnessTemplate = QString::fromUtf8(R"python(
import sys
import json
import time
import io
import traceback
from typing import List, Optional, Any
from dataclasses import is_dataclass, asdict

# --- UTILITIES ---
class TreeNode:
    def __init__(self, val=0, left=None, right=None):
        self.val = val
        self.left = left
        self.right = right

def to_tree_node(arr):
    if not arr or arr[0] is None:
        return None
    root = TreeNode(arr[0])
    queue = [root]
    i = 1
    while queue and i < len(arr):
        node = queue.pop(0)
        if i < len(arr) and arr[i] is not None:
            node.left = TreeNode(arr[i])
            queue.append(node.left)
        i += 1
        if i < len(arr) and arr[i] is not None:
            node.right = TreeNode(arr[i])
            queue.append(node.right)
        i += 1
    return root

def tree_node_to_array(root):
    if not root:
        return []
    res, queue = [], [root]
    while queue:
        node = queue.pop(0)
        if node:
            res.append(node.val)
            queue.append(node.left)
            queue.append(node.right)
        else:
            res.append(None)
    while res and res[-1] is None:
        res.pop()
    return res

class ListNode:
    def __init__(self, val=0, next=None):
        self.val = val
        self.next = next

def to_list_node(arr: List[int]) -> Optional[ListNode]:
    if not arr:
        return None
    head = ListNode(arr[0])
    cur = head
    for v in arr[1:]:
        cur.next = ListNode(v)
        cur = cur.next
    return head

def list_node_to_array(head: Optional[ListNode]) -> List[int]:
    res = []
    while head:
        res.append(head.val)
        head = head.next
    return res

def to_json(obj: Any) -> str:
    seen = set()
    return _encode(obj, seen)

def _encode(obj: Any, seen: set[int]) -> str:
    if obj is None:
        return "null"
    if obj is True:
        return "true"
    if obj is False:
        return "false"
    if isinstance(obj, int):
        return str(obj)
    if isinstance(obj, float):
        return repr(obj)
    if isinstance(obj, str):
        return _quote(obj)
    if isinstance(obj, (bytes, bytearray, memoryview)):
        return _quote(bytes(obj).hex())
    try:
        import enum
        if isinstance(obj, enum.Enum):
            return _quote(obj.name)
    except Exception:
        pass
    oid = id(obj)
    if oid in seen:
        raise ValueError("Cycle detected while encoding to json")
    seen.add(oid)
    try:
        if isinstance(obj, dict):
            items = []
            for k, v in obj.items():
                if not isinstance(k, str):
                    k = str(k)
                items.append(_quote(k) + ":" + _encode(v, seen))
            return "{" + ",".join(items) + "}"
        if isinstance(obj, (list, tuple, set, frozenset)):
            return "[" + ",".join(_encode(x, seen) for x in obj) + "]"
        if is_dataclass(obj):
            return _encode(asdict(obj), seen)
        if hasattr(obj, "__dict__"):
            d = {}
            for k, v in obj.__dict__.items():
                if k.startswith("_"):
                    continue
                d[k] = v
            return _encode(d, seen)
        slots = getattr(obj, "__slots__", None)
        if slots:
            d = {}
            if isinstance(slots, str):
                slots = [slots]
            for k in slots:
                if k.startswith("_"):
                    continue
                if hasattr(obj, k):
                    d[k] = getattr(obj, k)
            return _encode(d, seen)
        return _quote(str(obj))
    finally:
        seen.remove(oid)

def _quote(s: str) -> str:
    out = ['"']
    for ch in s:
        o = ord(ch)
        if ch == '"':
            out.append('\\"')
        elif ch == "\\":
            out.append("\\\\")
        elif ch == "\b":
            out.append("\\b")
        elif ch == "\f":
            out.append("\\f")
        elif ch == "\n":
            out.append("\\n")
        elif ch == "\r":
            out.append("\\r")
        elif ch == "\t":
            out.append("\\t")
        elif o < 0x20:
            out.append("\\u%04x" % o)
        else:
            out.append(ch)
    out.append('"')
    return "".join(out)

def compare_ignore_order(a, b):
    def sort_recursive(obj):
        if isinstance(obj, list):
            return sorted([sort_recursive(x) for x in obj], key=lambda x: str(x))
        elif isinstance(obj, dict):
            return {k: sort_recursive(v) for k, v in obj.items()}
        else:
            return obj
    return sort_recursive(a) == sort_recursive(b)

def deserialize_val(val, val_type):
    if val is None:
        return None
    if isinstance(val, dict) and 'elemType' in val and 'value' in val:
        val = val['value']
    if val_type == 'list_node':
        return to_list_node(val)
    elif val_type == 'tree_node':
        return to_tree_node(val)
    elif val_type == 'array':
        return val
    else:
        return val

# --- USER SOLUTION ---
%1

# --- RUNNER ---
def run_all_tests():
    manifest = json.loads(%2)
    test_cases = manifest.get('tests', [])
    entry = manifest.get('entry', {})
    params_schema = entry.get('params', {})
    call_template = entry.get('call', {}).get('python3', '')
    judge_type = manifest.get('judge', {}).get('type', 'exact')
    
    oracle = manifest.get('oracle', {}).get('python3', {})
    oracle_code = oracle.get('checker', '')
    oracle_call = oracle.get('call', '')
    
    if oracle_code:
        exec(oracle_code, globals())

    eval_expr = call_template
    for p_name in params_schema.keys():
        eval_expr = eval_expr.replace(f"{{{p_name}}}", p_name)

    results = []
    for idx, tc in enumerate(test_cases):
        captured_stdout = io.StringIO()
        captured_stderr = io.StringIO()
        old_stdout = sys.stdout
        old_stderr = sys.stderr
        sys.stdout = captured_stdout
        sys.stderr = captured_stderr
        
        status = "Accepted"
        actual_str = ""
        elapsed_ms = 0
        
        try:
            local_vars = {}
            for p_name, p_schema in params_schema.items():
                p_type = p_schema.get('type', 'int')
                raw_val = tc['in'][p_name]
                local_vars[p_name] = deserialize_val(raw_val, p_type)
            
            start_time = time.perf_counter()
            res_val = eval(eval_expr, globals(), local_vars)
            end_time = time.perf_counter()
            elapsed_ms = int((end_time - start_time) * 1000)
            
            actual_json = to_json(res_val)
            actual_str = actual_json
            
            expected_val = tc.get('out')
            if expected_val is not None:
                expected_json = to_json(expected_val)
                actual_obj = json.loads(actual_json)
                expected_obj = json.loads(expected_json)
                if judge_type == 'ignore_order':
                    if not compare_ignore_order(actual_obj, expected_obj):
                        status = "Wrong Answer"
                else:
                    if actual_obj != expected_obj:
                        status = "Wrong Answer"
            elif oracle_code and oracle_call:
                oracle_expr = oracle_call.replace('{result}', 'res_val')
                oracle_vars = {**local_vars, 'res_val': res_val}
                passed = eval(oracle_expr, globals(), oracle_vars)
                if not passed:
                    status = "Wrong Answer"
            else:
                pass
                
        except Exception as e:
            sys.stdout = old_stdout
            sys.stderr = old_stderr
            status = "Runtime Error"
            actual_str = traceback.format_exc()
            
        finally:
            sys.stdout = old_stdout
            sys.stderr = old_stderr
            
        results.append({
            "status": status,
            "actual": actual_str,
            "expected": to_json(tc.get('out')) if tc.get('out') is not None else "",
            "elapsedMs": elapsed_ms,
            "stdout": captured_stdout.getvalue() + captured_stderr.getvalue()
        })
        
    print("SF_JSON_SUMMARY_START")
    print(json.dumps(results))
    print("SF_JSON_SUMMARY_END")
run_all_tests()
)python");

    QString escapedManifest = manifestJson;
    escapedManifest.replace(QLatin1String("\\"), QLatin1String("\\\\"));
    escapedManifest.replace(QLatin1String("\""), QLatin1String("\\\""));
    
    int offset = harnessTemplate.left(harnessTemplate.indexOf("%1")).count('\n');
    QString fullExecutionCode = harnessTemplate
        .arg(code)
        .arg(QLatin1String("\"") + escapedManifest + QLatin1String("\""));
        
    EmbeddedRunner::Result r = m_pythonRunner->execute(fullExecutionCode, "", &m_stopRequested);
    
    if (r.exitCode != 0 && r.output.isEmpty()) {
        emit compilationError(OutputNormalizer::normalizeError(r.error.isEmpty() ? "Execution failed with non-zero exit code" : r.error, "python", offset));
        return;
    }
    
    QString output = r.output;
    int startIdx = output.indexOf("SF_JSON_SUMMARY_START");
    int endIdx = output.indexOf("SF_JSON_SUMMARY_END");
    
    if (startIdx == -1 || endIdx == -1) {
        emit compilationError("Harness output did not contain test summary. Output:\n" + output + "\nError:\n" + OutputNormalizer::normalizeError(r.error, "python", offset));
        return;
    }
    
    startIdx += QString("SF_JSON_SUMMARY_START").length();
    QString jsonStr = output.mid(startIdx, endIdx - startIdx).trimmed();
    
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonStr.toUtf8(), &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        emit systemError("Failed to parse test results JSON: " + parseError.errorString() + "\nRaw text: " + jsonStr);
        return;
    }
    
    QJsonArray results = doc.array();
    for (int i = 0; i < results.size(); ++i) {
        QJsonObject resObj = results[i].toObject();
        QString status = resObj["status"].toString();
        QString actual = resObj["actual"].toString();
        if (status == "Runtime Error") {
            actual = OutputNormalizer::normalizeError(actual, "python", offset);
        }
        QString expected = resObj["expected"].toString();
        qint64 elapsedMs = resObj["elapsedMs"].toInt();
        QString caseStdout = resObj["stdout"].toString();
        
        int targetIndex = (singleTestIndex >= 0) ? singleTestIndex : i;
        
        if (!caseStdout.isEmpty()) {
            actual = "--- Stdout Output ---\n" + caseStdout + "\n--- Return Value ---\n" + actual;
        }
        
        emit progress(i + 1, results.size());
        emit testResult(targetIndex, status, actual, expected, elapsedMs);
    }
}

void CodeRunner::runCppTestsBatch(const QString &code, const QJsonObject &manifest, int singleTestIndex) {
    QJsonObject runManifest = manifest;
    if (singleTestIndex >= 0) {
        QJsonArray allTests = manifest["tests"].toArray();
        if (singleTestIndex < allTests.size()) {
            QJsonArray singleTestArray;
            singleTestArray.append(allTests[singleTestIndex]);
            runManifest["tests"] = singleTestArray;
        }
    }
    
    QJsonObject entry = manifest["entry"].toObject();
    QJsonObject params = entry["params"].toObject();
    QString cppCall = entry["call"].toObject()["cpp"].toString();
    QString judgeType = manifest["judge"].toObject()["type"].toString("exact");
    
    QString mainCpp = R"(
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <optional>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <nlohmann/json.hpp>

using namespace std;
using _JSON_ = nlohmann::json;

)";

    mainCpp += R"_UTILITIES_(

template<typename T> T& lv(T&& x) { return x; }

struct _TEST_ {
    _JSON_ val;
};

inline void from_json(const _JSON_& j, _TEST_& t) {
    j.at("val").get_to(t.val);
}

inline char jsonToChar(const _JSON_& j) { return j.get<string>()[0]; }
inline int jsonToInt(const _JSON_& j) { return j.get<int>(); }
inline long long jsonToLong(const _JSON_& j) { return j.get<long long>(); }
inline double jsonToDouble(const _JSON_& j) { return j.get<double>(); }
inline float jsonToFloat(const _JSON_& j) { return j.get<float>(); }
inline bool jsonToBool(const _JSON_& j) { return j.get<bool>(); }
inline string jsonToString(const _JSON_& j) {
  return j.is_string() ? j.get<string>() : j.dump();
}

inline vector<int> jsonToIntArray(const _JSON_& j) {
  if (j.is_null()) return {};
  return j.get<vector<int> >();
}

inline vector<long long> jsonToLongArray(const _JSON_& j) {
  if (j.is_null()) return {};
  return j.get<vector<long long> >();
}

inline vector<double> jsonToDoubleArray(const _JSON_& j) {
  if (j.is_null()) return {};
  return j.get<vector<double> >();
}

inline vector<float> jsonToFloatArray(const _JSON_& j) {
  if (j.is_null()) return {};
  return j.get<vector<float> >();
}

inline vector<bool> jsonToBoolArray(const _JSON_& j) {
  if (j.is_null()) return {};
  return j.get<vector<bool> >();
}

inline vector<string> jsonToStringArray(const _JSON_& j) {
  if (j.is_null()) return {};
  return j.get<vector<string> >();
}

inline vector<optional<int>> jsonToOptionalIntArray(const _JSON_& j) {
  if (j.is_null()) return {};
  vector<optional<int>> out;
  out.reserve(j.size());
  for (const auto& x : j) {
    if (x.is_null()) {
      out.push_back(nullopt);
    } else {
      out.push_back(x.get<int>());
    }
  }
  return out;
}

inline vector<char> jsonToCharArray(const _JSON_& j) {
  if (j.is_null()) return {};
  vector<char> out;
  out.reserve(j.size());
  for (const auto& x : j) out.push_back(jsonToChar(x));
  return out;
}

inline vector<vector<int>> jsonToIntMatrix(const _JSON_& j) {
  if (j.is_null()) return {};
  return j.get<vector<vector<int> > >();
}

inline vector<vector<long long>> jsonToLongMatrix(const _JSON_& j) {
  if (j.is_null()) return {};
  return j.get<vector<vector<long long> > >();
}

inline vector<vector<double>> jsonToDoubleMatrix(const _JSON_& j) {
  if (j.is_null()) return {};
  return j.get<vector<vector<double> > >();
}

inline vector<vector<float>> jsonToFloatMatrix(const _JSON_& j) {
  if (j.is_null()) return {};
  return j.get<vector<vector<float> > >();
}

inline vector<vector<bool>> jsonToBoolMatrix(const _JSON_& j) {
  if (j.is_null()) return {};
  return j.get<vector<vector<bool> > >();
}

inline vector<vector<string>> jsonToStringMatrix(const _JSON_& j) {
  if (j.is_null()) return {};
  return j.get<vector<vector<string> > >();
}

inline vector<vector<char>> jsonToCharMatrix(const _JSON_& j) {
  if (j.is_null()) return {};
  vector<vector<char>> out;
  out.reserve(j.size());
  for (const auto& row : j) out.push_back(jsonToCharArray(row));
  return out;
}

struct ListNode {
  int val;
  ListNode *next;
  ListNode() : val(0), next(nullptr) {}
  ListNode(int x) : val(x), next(nullptr) {}
  ListNode(int x, ListNode *next) : val(x), next(next) {}
};

struct TreeNode {
  int val;
  TreeNode *left;
  TreeNode *right;
  TreeNode() : val(0), left(nullptr), right(nullptr) {}
  TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
  TreeNode(int x, TreeNode *left, TreeNode *right)
      : val(x), left(left), right(right) {}
};

inline ListNode *toListNode(const vector<int> &arr) {
  if (arr.empty())
    return nullptr;
  ListNode *head = new ListNode(arr[0]);
  ListNode *cur = head;
  for (int i = 1; i < (int)arr.size(); i++) {
    cur->next = new ListNode(arr[i]);
    cur = cur->next;
  }
  return head;
}

inline vector<int> listNodeToArray(ListNode *head) {
  vector<int> res;
  for (ListNode *cur = head; cur != nullptr; cur = cur->next)
    res.push_back(cur->val);
  return res;
}

inline vector<ListNode *> toLinkedLists(const vector<vector<int>> &arrs) {
  vector<ListNode *> res;
  res.reserve(arrs.size());
  for (const auto &arr : arrs)
    res.push_back(toListNode(arr));
  return res;
}

inline TreeNode *toTreeNode(const vector<optional<int>> &arr) {
  if (arr.empty() || !arr[0])
    return nullptr;
  TreeNode *root = new TreeNode(*arr[0]);
  queue<TreeNode *> q;
  q.push(root);
  int i = 1;
  while (!q.empty() && i < (int)arr.size()) {
    TreeNode *node = q.front();
    q.pop();
    if (i < (int)arr.size() && arr[i]) {
      node->left = new TreeNode(*arr[i]);
      q.push(node->left);
    }
    i++;
    if (i < (int)arr.size() && arr[i]) {
      node->right = new TreeNode(*arr[i]);
      q.push(node->right);
    }
    i++;
  }
  return root;
}

inline vector<optional<int>> treeNodeToArray(TreeNode *root) {
  vector<optional<int>> res;
  if (!root)
    return res;
  queue<TreeNode *> q;
  q.push(root);
  while (!q.empty()) {
    TreeNode *node = q.front();
    q.pop();
    if (node) {
      res.push_back(node->val);
      q.push(node->left);
      q.push(node->right);
    } else {
      res.push_back(nullopt);
    }
  }
  while (!res.empty() && !res.back())
    res.pop_back();
  return res;
}

inline vector<vector<optional<int>>> treeNodesArrayToVector(const vector<TreeNode *> &roots) {
  vector<vector<optional<int>>> res;
  res.reserve(roots.size());
  for (auto *root : roots) {
    res.push_back(treeNodeToArray(root));
  }
  return res;
}

inline string _jsonQuote(const string &s) {
  string r;
  r.reserve(s.size() + 2);
  r += '"';
  for (unsigned char c : s) {
    switch (c) {
    case '"':
      r += "\\\"";
      break;
    case '\\':
      r += "\\\\";
      break;
    case '\b':
      r += "\\b";
      break;
    case '\f':
      r += "\\f";
      break;
    case '\n':
      r += "\\n";
      break;
    case '\r':
      r += "\\r";
      break;
    case '\t':
      r += "\\t";
      break;
    default:
      if (c < 0x20) {
        char buf[8];
        snprintf(buf, sizeof(buf), "\\u%04x", c);
        r += buf;
      } else {
        r += (char)c;
      }
    }
  }
  r += '"';
  return r;
}

inline string toJson(bool v) { return v ? "true" : "false"; }
inline string toJson(int v) { return to_string(v); }
inline string toJson(long v) { return to_string(v); }
inline string toJson(long long v) { return to_string(v); }
inline string toJson(unsigned v) { return to_string(v); }
inline string toJson(unsigned long v) { return to_string(v); }
inline string toJson(unsigned long long v) { return to_string(v); }
inline string toJson(double v) {
  if (isnan(v) || isinf(v)) return "null";
  ostringstream oss;
  oss << setprecision(15) << v;
  string s = oss.str();
  if (s.find_first_of(".eE") == string::npos) s += ".0";
  return s;
}
inline string toJson(float v) { return toJson((double)v); }
inline string toJson(char v) { return _jsonQuote(string(1, v)); }
inline string toJson(const string &v) { return _jsonQuote(v); }
inline string toJson(const char *v) { return _jsonQuote(string(v)); }
inline string toJson(nullptr_t) { return "null"; }
inline string toJson(const optional<int> &v) {
  return v ? to_string(*v) : "null";
}

template <typename T> string toJson(const vector<T> &v) {
  string r = "[";
  for (int i = 0; i < (int)v.size(); i++) {
    if (i)
      r += ',';
    r += toJson(v[i]);
  }
  r += ']';
  return r;
}

inline string toJson(ListNode *head) { return toJson(listNodeToArray(head)); }
inline string toJson(TreeNode *root) { return toJson(treeNodeToArray(root)); }
\n)_UTILITIES_";
    
    int offset = mainCpp.count('\n');
    mainCpp += code + "\n";
    
    mainCpp += R"(
int main() {
    ifstream _TEST_JSON_FILE_("test.json");
    _JSON_ _TEST_JSON_;
    _TEST_JSON_FILE_ >> _TEST_JSON_;
    
    _JSON_ _RESULTS_ = _JSON_::array();
    
    for (auto& caseJson : _TEST_JSON_) {
        auto inputs = caseJson["in"];
)";

    for (auto it = params.begin(); it != params.end(); ++it) {
        QString pname = it.key();
        QString ptype = it.value().toObject()["type"].toString();
        if (ptype == "tree_node") {
            mainCpp += "        TreeNode* _" + pname + " = toTreeNode(inputs[\"" + pname + "\"].get<vector<optional<int>>>());\n";
        } else if (ptype == "list_node") {
            mainCpp += "        ListNode* _" + pname + " = toListNode(inputs[\"" + pname + "\"].get<vector<int>>());\n";
        } else if (ptype == "int") {
            mainCpp += "        int _" + pname + " = inputs[\"" + pname + "\"].get<int>();\n";
        } else if (ptype == "double") {
            mainCpp += "        double _" + pname + " = inputs[\"" + pname + "\"].get<double>();\n";
        } else if (ptype == "string") {
            mainCpp += "        string _" + pname + " = inputs[\"" + pname + "\"].get<string>();\n";
        } else if (ptype == "vector<int>") {
            mainCpp += "        vector<int> _" + pname + " = inputs[\"" + pname + "\"].get<vector<int>>();\n";
        } else if (ptype == "vector<string>") {
            mainCpp += "        vector<string> _" + pname + " = inputs[\"" + pname + "\"].get<vector<string>>();\n";
        }
    }
    
    QString evalExpr = cppCall;
    for (const QString &pname : params.keys()) {
        evalExpr.replace("{" + pname + "}", "_" + pname);
    }
    if (evalExpr.isEmpty()) {
        evalExpr = "0"; // fallback if missing call
    }
    
    mainCpp += "        auto res = " + evalExpr + ";\n";
    mainCpp += "        _JSON_ resObj;\n";
    mainCpp += "        try { resObj = _JSON_::parse(toJson(res)); } catch(...) { resObj = toJson(res); }\n";
    
    mainCpp += R"(
        _JSON_ resultItem;
        resultItem["actual"] = resObj;
        
        if (caseJson.contains("out")) {
            resultItem["expected"] = caseJson["out"];
        }
        _RESULTS_.push_back(resultItem);
    }
    
    cout << "SF_JSON_SUMMARY_START\n" << _RESULTS_.dump() << "\nSF_JSON_SUMMARY_END\n";
    return 0;
}
)";

    QString testJson = QString::fromUtf8(QJsonDocument(runManifest["tests"].toArray()).toJson(QJsonDocument::Compact));
    
    QElapsedTimer timer;
    timer.start();
    EmbeddedRunner::Result r = m_wasmRunnerCpp->execute(mainCpp, "", &m_stopRequested, {{"test.json", testJson}});
    
    if (r.exitCode != 0 && r.output.isEmpty()) {
        emit compilationError(OutputNormalizer::normalizeError(r.error.isEmpty() ? "Execution failed with non-zero exit code" : r.error, "cpp", offset));
        return;
    }
    
    QString output = r.output;
    int startIdx = output.indexOf("SF_JSON_SUMMARY_START");
    int endIdx = output.indexOf("SF_JSON_SUMMARY_END");
    
    if (startIdx == -1 || endIdx == -1) {
        emit compilationError("Harness output did not contain test summary. Output:\n" + output + "\nError:\n" + OutputNormalizer::normalizeError(r.error, "cpp", offset));
        return;
    }
    
    startIdx += QString("SF_JSON_SUMMARY_START").length();
    QString jsonStr = output.mid(startIdx, endIdx - startIdx).trimmed();
    
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonStr.toUtf8(), &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        emit systemError("Failed to parse test results JSON: " + parseError.errorString() + "\nRaw text: " + jsonStr);
        return;
    }
    
    QJsonArray results = doc.array();
    QJsonArray expectedResults = runManifest["tests"].toArray();
    for (int i = 0; i < results.size(); ++i) {
        QJsonObject resObj = results[i].toObject();
        QString actualStr = QString::fromUtf8(QJsonDocument(resObj["actual"].toArray()).toJson(QJsonDocument::Compact));
        if (resObj["actual"].isString()) actualStr = resObj["actual"].toString();
        else if (resObj["actual"].isDouble()) actualStr = QString::number(resObj["actual"].toDouble());
        else if (resObj["actual"].isBool()) actualStr = resObj["actual"].toBool() ? "true" : "false";
        else if (resObj["actual"].isNull()) actualStr = "null";
        else actualStr = QString::fromUtf8(QJsonDocument(resObj["actual"].toArray()).toJson(QJsonDocument::Compact));
        
        QString expectedStr = "";
        if (resObj.contains("expected")) {
            if (resObj["expected"].isString()) expectedStr = resObj["expected"].toString();
            else if (resObj["expected"].isDouble()) expectedStr = QString::number(resObj["expected"].toDouble());
            else if (resObj["expected"].isBool()) expectedStr = resObj["expected"].toBool() ? "true" : "false";
            else if (resObj["expected"].isNull()) expectedStr = "null";
            else expectedStr = QString::fromUtf8(QJsonDocument(resObj["expected"].toArray()).toJson(QJsonDocument::Compact));
        }
        
        QString status = (actualStr == expectedStr) ? "Accepted" : "Wrong Answer";
        
        int targetIndex = (singleTestIndex >= 0) ? singleTestIndex : i;
        emit progress(i + 1, results.size());
        emit testResult(targetIndex, status, actualStr, expectedStr, r.exitCode == 0 ? timer.elapsed() / results.size() : 0);
    }
}

void CodeRunner::runJavascriptTestsBatch(const QString &code, const QJsonObject &manifest, int singleTestIndex) {
    QJsonObject runManifest = manifest;
    if (singleTestIndex >= 0) {
        QJsonArray allTests = manifest["tests"].toArray();
        if (singleTestIndex < allTests.size()) {
            QJsonArray singleTestArray;
            singleTestArray.append(allTests[singleTestIndex]);
            runManifest["tests"] = singleTestArray;
        }
    }
    
    QJsonDocument manifestDoc(runManifest);
    QString manifestJson = QString::fromUtf8(manifestDoc.toJson(QJsonDocument::Compact));
    
    QString harnessTemplate = QString::fromUtf8(R"javascript(
// --- UTILITIES ---
function TreeNode(val, left, right) {
    this.val = (val===undefined ? 0 : val)
    this.left = (left===undefined ? null : left)
    this.right = (right===undefined ? null : right)
}

function to_tree_node(arr) {
    if (!arr || arr.length === 0 || arr[0] === null) return null;
    let root = new TreeNode(arr[0]);
    let queue = [root];
    let i = 1;
    while (queue.length > 0 && i < arr.length) {
        let node = queue.shift();
        if (i < arr.length && arr[i] !== null) {
            node.left = new TreeNode(arr[i]);
            queue.push(node.left);
        }
        i++;
        if (i < arr.length && arr[i] !== null) {
            node.right = new TreeNode(arr[i]);
            queue.push(node.right);
        }
        i++;
    }
    return root;
}

function ListNode(val, next) {
    this.val = (val===undefined ? 0 : val)
    this.next = (next===undefined ? null : next)
}

function to_list_node(arr) {
    if (!arr || arr.length === 0) return null;
    let head = new ListNode(arr[0]);
    let cur = head;
    for (let i = 1; i < arr.length; i++) {
        cur.next = new ListNode(arr[i]);
        cur = cur.next;
    }
    return head;
}

function deserialize_val(val, val_type) {
    if (val === null) return null;
    if (val_type === 'list_node') return to_list_node(val);
    if (val_type === 'tree_node') return to_tree_node(val);
    return val;
}

// --- USER SOLUTION ---
%1

// --- RUNNER ---
function run_all_tests() {
    const manifest = JSON.parse('%2');
    const test_cases = manifest.tests || [];
    const entry = manifest.entry || {};
    const params_schema = entry.params || {};
    let call_template = (entry.call && entry.call.javascript) ? entry.call.javascript : '';
    const judge_type = (manifest.judge && manifest.judge.type) ? manifest.judge.type : 'exact';

    let eval_expr = call_template;
    for (const p_name in params_schema) {
        eval_expr = eval_expr.split('{' + p_name + '}').join(p_name);
    }

    const results = [];
    
    // QuickJS override console.log to capture stdout
    let stdout_cap = "";
    const original_log = console.log;
    console.log = function(...args) {
        stdout_cap += args.map(a => String(a)).join(" ") + "\n";
        original_log.apply(console, args);
    };

    for (let i = 0; i < test_cases.length; i++) {
        const tc = test_cases[i];
        stdout_cap = "";
        
        let status = "Accepted";
        let actual_str = "";
        let elapsed_ms = 0;
        
        try {
            let local_vars_js = "";
            for (const p_name in params_schema) {
                const p_type = params_schema[p_name].type || 'int';
                const raw_val = tc.in[p_name];
                const deser = deserialize_val(raw_val, p_type);
                local_vars_js += `let ${p_name} = ${JSON.stringify(deser)};\n`;
                if (p_type === 'tree_node') {
                   local_vars_js += `${p_name} = to_tree_node(${p_name});\n`;
                } else if (p_type === 'list_node') {
                   local_vars_js += `${p_name} = to_list_node(${p_name});\n`;
                }
            }
            
            let start_time = Date.now();
            let res_val = eval(local_vars_js + eval_expr);
            elapsed_ms = Date.now() - start_time;
            
            actual_str = JSON.stringify(res_val);
            if (actual_str === undefined) actual_str = "null";
            
            const expected_val = tc.out;
            if (expected_val !== undefined) {
                if (JSON.stringify(res_val) !== JSON.stringify(expected_val)) {
                    status = "Wrong Answer";
                }
            }
        } catch (e) {
            status = "Runtime Error";
            if (e && e.stack) {
                actual_str = String(e) + "\n" + e.stack;
            } else {
                actual_str = String(e);
            }
        }
        
        results.push({
            status: status,
            actual: actual_str,
            expected: tc.out !== undefined ? JSON.stringify(tc.out) : "",
            elapsedMs: elapsed_ms,
            stdout: stdout_cap
        });
    }
    
    // Restore console.log just in case
    console.log = original_log;
    
    console.log("SF_JSON_SUMMARY_START");
    console.log(JSON.stringify(results));
    console.log("SF_JSON_SUMMARY_END");
}

run_all_tests();
)javascript");

    QString escapedManifest = manifestJson;
    escapedManifest.replace(QLatin1String("\\\\"), QLatin1String("\\\\\\\\"));
    escapedManifest.replace(QLatin1String("\\\""), QLatin1String("\\\\\\\""));
    
    int offset = harnessTemplate.left(harnessTemplate.indexOf("%1")).count('\n');
    QString fullExecutionCode = harnessTemplate
        .arg(code)
        .arg(escapedManifest); // No quotes around it because we put them in the JS parse
        
    EmbeddedRunner::Result r = m_javascriptRunner->execute(fullExecutionCode, "", &m_stopRequested);
    
    if (r.exitCode != 0 && r.output.isEmpty()) {
        emit compilationError(OutputNormalizer::normalizeError(r.error.isEmpty() ? "Execution failed with non-zero exit code" : r.error, "javascript", offset));
        return;
    }
    
    QString output = r.output;
    int startIdx = output.indexOf("SF_JSON_SUMMARY_START");
    int endIdx = output.indexOf("SF_JSON_SUMMARY_END");
    
    if (startIdx == -1 || endIdx == -1) {
        emit compilationError("Harness output did not contain test summary. Output:\n" + output + "\nError:\n" + OutputNormalizer::normalizeError(r.error, "javascript", offset));
        return;
    }
    
    startIdx += QString("SF_JSON_SUMMARY_START").length();
    QString jsonStr = output.mid(startIdx, endIdx - startIdx).trimmed();
    
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonStr.toUtf8(), &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        emit systemError("Failed to parse test results JSON: " + parseError.errorString() + "\nRaw text: " + jsonStr);
        return;
    }
    
    QJsonArray results = doc.array();
    for (int i = 0; i < results.size(); ++i) {
        QJsonObject resObj = results[i].toObject();
        QString status = resObj["status"].toString();
        QString actual = resObj["actual"].toString();
        if (status == "Runtime Error") {
            actual = OutputNormalizer::normalizeError(actual, "javascript", offset);
        }
        QString expected = resObj["expected"].toString();
        qint64 elapsedMs = resObj["elapsedMs"].toInt();
        QString caseStdout = resObj["stdout"].toString();
        
        int targetIndex = (singleTestIndex >= 0) ? singleTestIndex : i;
        
        if (!caseStdout.isEmpty()) {
            actual = "--- Stdout Output ---\n" + caseStdout + "\n--- Return Value ---\n" + actual;
        }
        
        emit progress(i + 1, results.size());
        emit testResult(targetIndex, status, actual, expected, elapsedMs);
    }
}

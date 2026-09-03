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
#include <QRegularExpression>

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
    bool expected = false;
    if (!m_running.compare_exchange_strong(expected, true)) {
        emit systemError("Already running");
        return;
    }

    m_stopRequested = false;
    emit started();

    std::thread([this, code, languageId, problemPath]() {

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
    
    int timeLimit = calculateSmartTimeLimit(languageId);

    if (languageId == "python" && !m_currentManifest.isEmpty()) {
        runPythonTestsBatch(code, m_currentManifest, -1, timeLimit);
    } else if ((languageId == "cpp" || languageId == "c++" || languageId == "c") && !m_currentManifest.isEmpty()) {
        runCppTestsBatch(code, m_currentManifest, -1, languageId, timeLimit);
    } else if ((languageId == "javascript" || languageId == "js") && !m_currentManifest.isEmpty()) {
        runJavascriptTestsBatch(code, m_currentManifest, -1, timeLimit);
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
            executeTestRaw(runner, fullCode, tests[i].toObject(), i, offset, timeLimit / qMax(1, (int)tests.size()));
        }
    }

    m_running = false;
    emit finished();
    }).detach();
}

void CodeRunner::runSingleTest(const QString &code, const QString &languageId,
                               int testIndex, const QString &problemPath) {
    bool expected = false;
    if (!m_running.compare_exchange_strong(expected, true)) {
        emit systemError("Already running");
        return;
    }

    m_stopRequested = false;
    emit started();

    std::thread([this, code, languageId, testIndex, problemPath]() {

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

    int timeLimit = calculateSmartTimeLimit(languageId);

    if (languageId == "python" && !m_currentManifest.isEmpty()) {
        runPythonTestsBatch(code, m_currentManifest, testIndex, timeLimit);
    } else if ((languageId == "cpp" || languageId == "c++" || languageId == "c") && !m_currentManifest.isEmpty()) {
        runCppTestsBatch(code, m_currentManifest, testIndex, languageId, timeLimit);
    } else if ((languageId == "javascript" || languageId == "js") && !m_currentManifest.isEmpty()) {
        runJavascriptTestsBatch(code, m_currentManifest, testIndex, timeLimit);
    } else {
        QString fullCode = code;
        int offset = 0;
        if (schema.isValid()) {
            QString header = DriverGenerator::generateHeader(schema, languageId);
            offset = header.count('\n');
            fullCode = header + fullCode + DriverGenerator::generateDriver(schema, languageId);
        }

        executeTestRaw(runner, fullCode, tests[testIndex].toObject(), testIndex, offset, timeLimit / qMax(1, (int)tests.size()));
    }

    m_running = false;
    emit finished();
    }).detach();
}







void CodeRunner::runFreeCode(const QString &code, const QString &languageId) {
    // BUG-09: guard against concurrent execution (same guard as runCode/runSingleTest)
    bool expected = false;
    if (!m_running.compare_exchange_strong(expected, true)) {
        emit systemError("Already running");
        return;
    }
    m_stopRequested = false;

    std::thread([this, code, languageId]() {

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
    if (r.exitCode == 0 && r.error.isEmpty()) {
        emit freeCodeFinished(0, ms);
    } else {
        emit freeCodeFinished(r.exitCode != 0 ? r.exitCode : 1, ms);
    }

    m_running = false;
    emit finished();
    }).detach();
}

void CodeRunner::stop() {
    m_stopRequested = true;
}

bool CodeRunner::loadTestCases(const QString &problemPath, QJsonArray &tests, MethodSchema &schema) {
    qDebug() << "Loading test cases from:" << problemPath;
    
    auto parseDoc = [&](const QJsonDocument& doc) {
        m_currentProblem = doc.object();
        tests = m_currentProblem["testCases"].toArray();
        if (m_currentProblem.contains("method")) {
            schema = MethodSchema::fromJson(m_currentProblem["method"].toObject());
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
                                int index, int offset, int timeoutMs)
{
    QString input    = test["input"].toString();
    QString expected = test["expected"].toString().trimmed();

    QElapsedTimer timer;
    timer.start();
    
    EmbeddedRunner::Result r = runner->execute(code, input, &m_stopRequested, {}, timeoutMs);
    
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

void CodeRunner::runPythonTestsBatch(const QString &code, const QJsonObject &manifest, int singleTestIndex, int timeoutMs) {
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
    
    QString harnessTemplate = R"python(
import sys
import json
import time
import io
import traceback
from typing import List, Optional, Any

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

def _encode(obj: Any, seen: set) -> str:
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
_SF_USER_CODE_HERE_

# --- RUNNER ---
def run_all_tests():
    manifest = json.loads(_SF_MANIFEST_JSON_HERE_)
    test_cases = manifest.get('tests', [])
    entry = manifest.get('entry', {})
    params_schema = entry.get('params', {})
    call_dict = entry.get('call', {})
    call_template = call_dict.get('python3', call_dict.get('python', call_dict.get('python2', '')))
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
        
        extracted_expected = [None]
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
            
            exp_val = tc.get('out')
            if exp_val is None or judge_type == 'custom':
                if oracle_call:
                    try:
                        def trace_calls(frame, event, arg):
                            if event == "call":
                                if "expected" in frame.f_code.co_varnames:
                                    if hasattr(frame, 'f_trace_lines'):
                                        frame.f_trace_lines = False
                                    return trace_calls_local
                                return None
                            return None
                        def trace_calls_local(frame, event, arg):
                            if event == "return":
                                locals_dict = frame.f_locals
                                if "expected" in locals_dict:
                                    extracted_expected[0] = locals_dict["expected"]
                            return trace_calls_local
                        sys.settrace(trace_calls)
                        oracle_call_replaced = oracle_call.replace("{result}", "res")
                        is_correct = eval(oracle_call_replaced, globals(), {**local_vars, "res": res_val})
                        sys.settrace(None)
                        if not is_correct:
                            status = "Wrong Answer"
                    except Exception as e:
                        sys.settrace(None)
                        status = "System Error"
                        actual_str = f"Checker error: {str(e)}"
                else:
                    if res_val != exp_val:
                        status = "Wrong Answer"
            elif judge_type == 'unordered':
                if not compare_ignore_order(res_val, exp_val):
                    status = "Wrong Answer"
            else:
                exp_json = to_json(exp_val)
                if actual_json != exp_json:
                    status = "Wrong Answer"
                        
        except Exception as e:
            status = "Runtime Error"
            actual_str = traceback.format_exc()
            
        finally:
            sys.stdout = old_stdout
            sys.stderr = old_stderr
            
        results.append({
            "status": status,
            "actual": actual_str,
            "expected": to_json(extracted_expected[0]) if extracted_expected[0] is not None else (to_json(tc.get('out')) if tc.get('out') is not None else ""),
            "elapsedMs": elapsed_ms,
            "stdout": captured_stdout.getvalue() + captured_stderr.getvalue()
        })
        
    print("SF_JSON_SUMMARY_START")
    print(json.dumps(results))
    print("SF_JSON_SUMMARY_END")
run_all_tests()
)python";

    QString escapedManifest = manifestJson;
    escapedManifest.replace(QLatin1String("\\"), QLatin1String("\\\\"));
    escapedManifest.replace(QLatin1String("\""), QLatin1String("\\\""));
    
    int offset = 0; // offset is 0 because user code is in a separate file (solution.py)
    QString fullExecutionCode = harnessTemplate;
    fullExecutionCode.replace("_SF_MANIFEST_JSON_HERE_", QLatin1String("\"") + escapedManifest + QLatin1String("\""));
    
    QString dynamicExec = R"python(
with open("solution.py", "r", encoding="utf-8") as _sf_f:
    _sf_user_code_str = _sf_f.read()
_sf_code_obj = compile(_sf_user_code_str, "solution.py", "exec")
exec(_sf_code_obj, globals())
)python";

    fullExecutionCode.replace("_SF_USER_CODE_HERE_", dynamicExec);
    
    QFile dumpFile("/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/dump_python.py");
    if (dumpFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        dumpFile.write(fullExecutionCode.toUtf8());
        dumpFile.close();
    }
        
    QMap<QString, QString> addFiles;
    addFiles["solution.py"] = code;
    
    EmbeddedRunner::Result r = m_pythonRunner->execute(fullExecutionCode, "", &m_stopRequested, addFiles, timeoutMs);
    
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

void CodeRunner::runCppTestsBatch(const QString &code, const QJsonObject &manifest, int singleTestIndex, const QString &languageId, int timeoutMs) {
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
    if (languageId == "c") {
        cppCall.remove("Solution().");
        cppCall.remove("Solution::");
    }
    QString judgeType = manifest["judge"].toObject()["type"].toString("exact");
    
    QString mainCpp = R"(
#include "stdcpp.h"
#include <type_traits>

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
)_UTILITIES_";
    
    if (languageId == "c") {
        mainCpp += R"_SF_C_COMPAT_(
#include <cstdlib>
struct _SF_AutoCast {
    void* ptr;
    _SF_AutoCast(void* p) : ptr(p) {}
    template<typename U> operator U*() const { return static_cast<U*>(ptr); }
};
#define malloc(size) _SF_AutoCast(::malloc(size))
#define calloc(n, size) _SF_AutoCast(::calloc(n, size))
#define realloc(ptr, size) _SF_AutoCast(::realloc(ptr, size))
)_SF_C_COMPAT_";
        mainCpp += "\n";
    }
    
    int offset = mainCpp.count('\n');
    mainCpp += code + "\n";
    
    mainCpp += R"(
int main() {
    ifstream _TEST_JSON_FILE_("/src/test.json");
    if (!_TEST_JSON_FILE_.is_open()) { return 1; }
    string _TEST_JSON_STR_((istreambuf_iterator<char>(_TEST_JSON_FILE_)), istreambuf_iterator<char>());
    _JSON_ _TEST_JSON_ = _JSON_::parse(_TEST_JSON_STR_, nullptr, false);
    if (_TEST_JSON_.is_discarded()) { return 1; }
    
    _JSON_ _RESULTS_ = _JSON_::array();
    
    for (auto& caseJson : _TEST_JSON_) {
        cout << "DEBUG:" << caseJson.dump() << endl;
        auto inputs = caseJson["in"];
)";

    for (auto it = params.begin(); it != params.end(); ++it) {
        QString pname = it.key();
        QJsonObject pObj = it.value().toObject();
        QString ptype = pObj["type"].toString();
        if (ptype == "array") {
            QString itemType = pObj["items"].toObject()["type"].toString();
            if (itemType == "array") {
                QString nestedType = pObj["items"].toObject()["items"].toObject()["type"].toString();
                if (nestedType == "int") ptype = "vector<vector<int>>";
                else if (nestedType == "string") ptype = "vector<vector<string>>";
                else if (nestedType == "double") ptype = "vector<vector<double>>";
                else if (nestedType == "bool") ptype = "vector<vector<bool>>";
                else if (nestedType == "char") ptype = "vector<vector<char>>";
            } else {
                if (itemType == "int") ptype = "vector<int>";
                else if (itemType == "string") ptype = "vector<string>";
                else if (itemType == "double") ptype = "vector<double>";
                else if (itemType == "bool") ptype = "vector<bool>";
                else if (itemType == "char") ptype = "vector<char>";
            }
        }
        
        if (ptype == "tree_node") {
            mainCpp += "        TreeNode* _" + pname + " = toTreeNode(jsonToOptionalIntArray(inputs[\"" + pname + "\"]));\n";
        } else if (ptype == "list_node") {
            mainCpp += "        ListNode* _" + pname + " = toListNode(jsonToIntArray(inputs[\"" + pname + "\"]));\n";
        } else if (ptype == "int" || ptype == "integer") {
            mainCpp += "        int _" + pname + " = (inputs.contains(\"" + pname + "\") && inputs[\"" + pname + "\"].is_number()) ? inputs[\"" + pname + "\"].get<int>() : 0;\n";
        } else if (ptype == "double" || ptype == "float") {
            mainCpp += "        double _" + pname + " = (inputs.contains(\"" + pname + "\") && inputs[\"" + pname + "\"].is_number()) ? inputs[\"" + pname + "\"].get<double>() : 0.0;\n";
        } else if (ptype == "string") {
            mainCpp += "        string _" + pname + " = (inputs.contains(\"" + pname + "\") && inputs[\"" + pname + "\"].is_string()) ? inputs[\"" + pname + "\"].get<string>() : \"\";\n";
        } else if (ptype == "bool" || ptype == "boolean") {
            mainCpp += "        bool _" + pname + " = (inputs.contains(\"" + pname + "\") && inputs[\"" + pname + "\"].is_boolean()) ? inputs[\"" + pname + "\"].get<bool>() : false;\n";
        } else if (ptype == "vector<int>") {
            mainCpp += "        vector<int> _" + pname + " = inputs.contains(\"" + pname + "\") ? jsonToIntArray(inputs[\"" + pname + "\"]) : vector<int>();\n";
        } else if (ptype == "vector<string>") {
            mainCpp += "        vector<string> _" + pname + " = inputs.contains(\"" + pname + "\") ? jsonToStringArray(inputs[\"" + pname + "\"]) : vector<string>();\n";
        } else if (ptype == "vector<double>") {
            mainCpp += "        vector<double> _" + pname + " = inputs.contains(\"" + pname + "\") ? jsonToDoubleArray(inputs[\"" + pname + "\"]) : vector<double>();\n";
        } else if (ptype == "vector<char>") {
            mainCpp += "        vector<char> _" + pname + " = inputs.contains(\"" + pname + "\") ? jsonToCharArray(inputs[\"" + pname + "\"]) : vector<char>();\n";
        } else if (ptype == "vector<bool>") {
            mainCpp += "        vector<bool> _" + pname + " = inputs.contains(\"" + pname + "\") ? jsonToBoolArray(inputs[\"" + pname + "\"]) : vector<bool>();\n";
        } else if (ptype == "vector<vector<int>>") {
            mainCpp += "        vector<vector<int>> _" + pname + " = inputs.contains(\"" + pname + "\") ? jsonToIntMatrix(inputs[\"" + pname + "\"]) : vector<vector<int>>();\n";
        } else if (ptype == "vector<vector<string>>") {
            mainCpp += "        vector<vector<string>> _" + pname + " = inputs.contains(\"" + pname + "\") ? jsonToStringMatrix(inputs[\"" + pname + "\"]) : vector<vector<string>>();\n";
        } else if (ptype == "vector<vector<double>>") {
            mainCpp += "        vector<vector<double>> _" + pname + " = inputs.contains(\"" + pname + "\") ? jsonToDoubleMatrix(inputs[\"" + pname + "\"]) : vector<vector<double>>();\n";
        } else if (ptype == "vector<vector<char>>") {
            mainCpp += "        vector<vector<char>> _" + pname + " = inputs.contains(\"" + pname + "\") ? jsonToCharMatrix(inputs[\"" + pname + "\"]) : vector<vector<char>>();\n";
        } else if (ptype == "vector<vector<bool>>") {
            mainCpp += "        vector<vector<bool>> _" + pname + " = inputs.contains(\"" + pname + "\") ? jsonToBoolMatrix(inputs[\"" + pname + "\"]) : vector<vector<bool>>();\n";
        }
    }
    
    QString evalExpr = cppCall;
    QString preCall = "";
    QString postCall = "";
    bool hasReturnSize = false;

    if (languageId == "c") {
        QString funcName = cppCall;
        int pIndex = funcName.indexOf('(');
        if (pIndex != -1) funcName = funcName.left(pIndex).trimmed();
        
        QRegularExpression rx(funcName + "\\s*\\(([^)]*)\\)");
        QRegularExpressionMatch match = rx.match(code);
        if (match.hasMatch()) {
            QString argsStr = match.captured(1);
            QStringList args = argsStr.split(',');
            QStringList cCallArgs;
            for (QString arg : args) {
                arg = arg.trimmed();
                int lastSpace = arg.lastIndexOf(QRegularExpression("\\s|\\*"));
                QString argName = arg.mid(lastSpace + 1).trimmed();
                
                if (argName == "returnSize") {
                    preCall += "        int _sf_returnSize = 0;\n";
                    cCallArgs << "&_sf_returnSize";
                    hasReturnSize = true;
                } else if (argName == "returnColumnSizes") {
                    preCall += "        int* _sf_returnColSizes = nullptr;\n";
                    cCallArgs << "&_sf_returnColSizes";
                } else if (argName.endsWith("Size")) {
                    QString base = argName.left(argName.length() - 4);
                    cCallArgs << "_" + base + ".size()";
                } else if (argName.endsWith("Sizes")) {
                    QString base = argName.left(argName.length() - 5);
                    preCall += "        vector<int> _sf_" + argName + ";\n";
                    preCall += "        for(auto& r : _" + base + ") _sf_" + argName + ".push_back(r.size());\n";
                    cCallArgs << "_sf_" + argName + ".data()";
                } else {
                    if (params.contains(argName) && params[argName].toObject()["type"].toString() == "array") {
                        cCallArgs << "_" + argName + ".data()";
                    } else {
                        cCallArgs << "_" + argName;
                    }
                }
            }
            evalExpr = funcName + "(" + cCallArgs.join(", ") + ")";
        } else {
            for (const QString &pname : params.keys()) evalExpr.replace("{" + pname + "}", "_" + pname);
        }
    } else {
        for (const QString &pname : params.keys()) {
            evalExpr.replace("{" + pname + "}", "_" + pname);
        }
    }

    if (evalExpr.isEmpty()) {
        evalExpr = "0"; // fallback if missing call
    }
    
    mainCpp += preCall;
    mainCpp += "        auto res = " + evalExpr + ";\n";
    mainCpp += "        _JSON_ resObj;\n";
    
    if (hasReturnSize) {
        mainCpp += "        if (res) {\n";
        mainCpp += "            vector<remove_pointer<decltype(res)>::type> _sf_res_vec(res, res + _sf_returnSize);\n";
        mainCpp += "            resObj = _JSON_::parse(toJson(_sf_res_vec), nullptr, false);\n";
        mainCpp += "            if (resObj.is_discarded()) { resObj = toJson(_sf_res_vec); }\n";
        mainCpp += "        } else {\n";
        mainCpp += "            resObj = _JSON_();\n";
        mainCpp += "        }\n";
    } else {
        mainCpp += "        resObj = _JSON_::parse(toJson(res), nullptr, false);\n";
        mainCpp += "        if (resObj.is_discarded()) { resObj = toJson(res); }\n";
    }
    
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
    
    QFile dumpFile("/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/dump_cpp.cpp");
    if (dumpFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        dumpFile.write(mainCpp.toUtf8());
        dumpFile.close();
    }
    
    QElapsedTimer timer;
    timer.start();
    EmbeddedRunner *runner = (languageId == "c") ? (EmbeddedRunner*)m_wasmRunnerC : (EmbeddedRunner*)m_wasmRunnerCpp;
    EmbeddedRunner::Result r = runner->execute(mainCpp, "", &m_stopRequested, {{"test.json", testJson}}, timeoutMs);
    
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
    QJsonArray statuses = evaluateResultsWithPython(runManifest, results);
    
    for (int i = 0; i < results.size(); ++i) {
        QJsonObject resObj = results[i].toObject();
        QString actualStr;
        if (resObj["actual"].isString()) actualStr = resObj["actual"].toString();
        else if (resObj["actual"].isDouble()) actualStr = QString::number(resObj["actual"].toDouble());
        else if (resObj["actual"].isBool()) actualStr = resObj["actual"].toBool() ? "true" : "false";
        else if (resObj["actual"].isNull()) actualStr = "null";
        else if (resObj["actual"].isObject()) actualStr = QString::fromUtf8(QJsonDocument(resObj["actual"].toObject()).toJson(QJsonDocument::Compact));
        else actualStr = QString::fromUtf8(QJsonDocument(resObj["actual"].toArray()).toJson(QJsonDocument::Compact));
        
        QString expectedStr = "";
        if (resObj.contains("expected")) {
            if (resObj["expected"].isString()) expectedStr = resObj["expected"].toString();
            else if (resObj["expected"].isDouble()) expectedStr = QString::number(resObj["expected"].toDouble());
            else if (resObj["expected"].isBool()) expectedStr = resObj["expected"].toBool() ? "true" : "false";
            else if (resObj["expected"].isNull()) expectedStr = "null";
            else expectedStr = QString::fromUtf8(QJsonDocument(resObj["expected"].toArray()).toJson(QJsonDocument::Compact));
        }
        
        QString status = "System Error";
        if (i < statuses.size()) {
            if (statuses[i].isObject()) {
                QJsonObject obj = statuses[i].toObject();
                status = obj["status"].toString();
                if (obj.contains("expected") && !obj["expected"].isNull()) {
                    if (obj["expected"].isString()) expectedStr = obj["expected"].toString();
                    else if (obj["expected"].isDouble()) expectedStr = QString::number(obj["expected"].toDouble());
                    else if (obj["expected"].isBool()) expectedStr = obj["expected"].toBool() ? "true" : "false";
                    else expectedStr = QString::fromUtf8(QJsonDocument(obj["expected"].toArray()).toJson(QJsonDocument::Compact));
                }
            } else {
                status = statuses[i].toString();
            }
        }
        
        int targetIndex = (singleTestIndex >= 0) ? singleTestIndex : i;
        emit progress(i + 1, results.size());
        emit testResult(targetIndex, status, actualStr, expectedStr, r.exitCode == 0 ? timer.elapsed() / results.size() : 0);
    }
}

void CodeRunner::runJavascriptTestsBatch(const QString &code, const QJsonObject &manifest, int singleTestIndex, int timeoutMs) {
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

function from_list_node(node) {
    let arr = [];
    while (node !== null && node !== undefined) {
        arr.push(node.val);
        node = node.next;
    }
    return arr;
}

function from_tree_node(root) {
    if (!root) return [];
    let arr = [];
    let queue = [root];
    while (queue.length > 0) {
        let node = queue.shift();
        if (node !== null) {
            arr.push(node.val);
            queue.push(node.left);
            queue.push(node.right);
        } else {
            arr.push(null);
        }
    }
    while (arr.length > 0 && arr[arr.length - 1] === null) {
        arr.pop();
    }
    return arr;
}

function serialize_val(val, val_type) {
    if (val === null || val === undefined) return null;
    if (val_type === 'list_node') return from_list_node(val);
    if (val_type === 'tree_node') return from_tree_node(val);
    return val;
}

const listNodeToArray = from_list_node;
const treeNodeToArray = from_tree_node;

// --- USER SOLUTION ---
_SF_USER_CODE_HERE_

// --- RUNNER ---
function run_all_tests() {
    const manifest = _SF_MANIFEST_JSON_HERE_;
    const test_cases = manifest.tests || [];
    const entry = manifest.entry || {};
    const params_schema = entry.params || {};
    let call_template = '';
    if (entry.call) {
        call_template = entry.call.javascript || entry.call.js || entry.call.typescript || '';
    }
    const judge_type = (manifest.judge && manifest.judge.type) ? manifest.judge.type : 'exact';

    let eval_expr = call_template;
    for (const p_name in params_schema) {
        eval_expr = eval_expr.split('{' + p_name + '}').join(p_name);
    }

    const results = [];
    
    // QuickJS override console.log to capture stdout
    const original_log = console.log;
    let stdout_cap = "";
    console.log = function(...args) {
        stdout_cap += args.join(" ") + "\n";
    };

    for (let _test_idx = 0; _test_idx < test_cases.length; _test_idx++) {
        const tc = test_cases[_test_idx];
        stdout_cap = "";
        
        let actual_str = "";
        let status = "Accepted";
        let elapsed_ms = 0;
        
        try {
            // Assign parameters securely
            for (const p_name in params_schema) {
                let p_val = tc.in[p_name];
                p_val = deserialize_val(p_val, params_schema[p_name].type);
                eval("var " + p_name + " = p_val;");
            }
            
            const t0 = Date.now();
            let res = eval(eval_expr);
            elapsed_ms = Date.now() - t0;
            
            const ret_type = (entry.return && entry.return.type) ? entry.return.type : 'any';
            res = serialize_val(res, ret_type);
            
            actual_str = res !== undefined ? res : null;
            const exp_str = tc.out !== undefined ? JSON.stringify(tc.out) : "";
            
            if (res !== undefined && JSON.stringify(res) !== exp_str) {
                status = "Wrong Answer";
            }
        } catch (e) {
            status = "Runtime Error";
            actual_str = e.toString();
            if (e.stack) {
                actual_str += "\n" + e.stack;
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

    int offset = harnessTemplate.left(harnessTemplate.indexOf("_SF_USER_CODE_HERE_")).count('\n');
    QString fullExecutionCode = harnessTemplate;
    fullExecutionCode.replace("_SF_MANIFEST_JSON_HERE_", manifestJson);
    fullExecutionCode.replace("_SF_USER_CODE_HERE_", code);
    
    QFile dumpJs("/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/dump_js.js");
    if (dumpJs.open(QIODevice::WriteOnly | QIODevice::Text)) {
        dumpJs.write(fullExecutionCode.toUtf8());
        dumpJs.close();
    }
        
    EmbeddedRunner::Result r = m_javascriptRunner->execute(fullExecutionCode, "", &m_stopRequested, {}, timeoutMs);
    
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
    QJsonArray statuses = evaluateResultsWithPython(runManifest, results);
    
    for (int i = 0; i < results.size(); ++i) {
        QJsonObject resObj = results[i].toObject();
        QString status = resObj["status"].toString();
        QString expectedStr = resObj["expected"].toString();
        
        if (status == "Wrong Answer" || status == "Accepted") {
            status = "System Error";
            if (i < statuses.size()) {
                if (statuses[i].isObject()) {
                    QJsonObject obj = statuses[i].toObject();
                    status = obj["status"].toString();
                    if (obj.contains("expected") && !obj["expected"].isNull()) {
                        if (obj["expected"].isString()) expectedStr = obj["expected"].toString();
                        else if (obj["expected"].isDouble()) expectedStr = QString::number(obj["expected"].toDouble());
                        else if (obj["expected"].isBool()) expectedStr = obj["expected"].toBool() ? "true" : "false";
                        else expectedStr = QString::fromUtf8(QJsonDocument(obj["expected"].toArray()).toJson(QJsonDocument::Compact));
                    }
                } else {
                    status = statuses[i].toString();
                }
            }
        }
        
        QString actual;
        if (resObj["actual"].isString()) actual = resObj["actual"].toString();
        else if (resObj["actual"].isDouble()) actual = QString::number(resObj["actual"].toDouble());
        else if (resObj["actual"].isBool()) actual = resObj["actual"].toBool() ? "true" : "false";
        else if (resObj["actual"].isNull()) actual = "null";
        else if (resObj["actual"].isObject()) actual = QString::fromUtf8(QJsonDocument(resObj["actual"].toObject()).toJson(QJsonDocument::Compact));
        else actual = QString::fromUtf8(QJsonDocument(resObj["actual"].toArray()).toJson(QJsonDocument::Compact));
        
        if (resObj["status"].toString() == "Runtime Error") {
            actual = OutputNormalizer::normalizeError(resObj["actual"].toString(), "javascript", offset);
        }
        
        QString expected = expectedStr.isEmpty() ? resObj["expected"].toString() : expectedStr;
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

QJsonArray CodeRunner::evaluateResultsWithPython(const QJsonObject &manifest, const QJsonArray &results) {
    QJsonObject evalData;
    evalData["manifest"] = manifest;
    evalData["results"] = results;
    
    QString evalDataJson = QString::fromUtf8(QJsonDocument(evalData).toJson(QJsonDocument::Compact));
    
    QString escapedData = evalDataJson;
    escapedData.replace(QLatin1String("\\"), QLatin1String("\\\\"));
    escapedData.replace(QLatin1String("\""), QLatin1String("\\\""));
    
    QString evaluatorCode = QString::fromUtf8(R"python(
import json
import sys

def compare_ignore_order(a, b):
    if isinstance(a, list) and isinstance(b, list):
        if len(a) != len(b): return False
        b_copy = b[:]
        for item in a:
            found = False
            for i, b_item in enumerate(b_copy):
                if compare_ignore_order(item, b_item):
                    b_copy.pop(i)
                    found = True
                    break
            if not found: return False
        return True
    elif isinstance(a, dict) and isinstance(b, dict):
        if len(a) != len(b): return False
        for k, v in a.items():
            if k not in b: return False
            if not compare_ignore_order(v, b[k]): return False
        return True
    else:
        return a == b

def evaluate():
    data_str = "_SF_DATA_JSON_HERE_"
    data = json.loads(data_str)
    
    manifest = data.get('manifest', {})
    results = data.get('results', [])
    
    judge_type = manifest.get('judge', {}).get('type', 'exact')
    oracle = manifest.get('oracle', {}).get('python3', {})
    oracle_code = oracle.get('checker', '')
    oracle_call = oracle.get('call', '')
    
    if oracle_code:
        exec(oracle_code, globals())
        
    statuses = []
    for i, res in enumerate(results):
        actual = res.get('actual')
        tc = manifest.get('tests', [])[i]
        exp = tc.get('out')
        
        status = "Accepted"
        extracted_expected = [None]
        if exp is None or judge_type == 'custom':
            if oracle_call:
                local_vars = {}
                for p_name, p_val in tc.get('in', {}).items():
                    local_vars[p_name] = p_val
                try:
                    def trace_calls(frame, event, arg):
                        if event == "call":
                            if "expected" in frame.f_code.co_varnames:
                                if hasattr(frame, 'f_trace_lines'):
                                    frame.f_trace_lines = False
                                return trace_calls_local
                            return None
                        return None
                    def trace_calls_local(frame, event, arg):
                        if event == "return":
                            locals_dict = frame.f_locals
                            if "expected" in locals_dict:
                                extracted_expected[0] = locals_dict["expected"]
                        return trace_calls_local
                    sys.settrace(trace_calls)
                    oracle_call_replaced = oracle_call.replace("{result}", "res")
                    is_correct = eval(oracle_call_replaced, globals(), {**local_vars, "res": actual})
                    sys.settrace(None)
                    if not is_correct: status = "Wrong Answer"
                except Exception as e:
                    sys.settrace(None)
                    status = "System Error"
            else:
                if actual != exp: status = "Wrong Answer"
        elif judge_type == 'unordered':
            if not compare_ignore_order(actual, exp): status = "Wrong Answer"
        else:
            if json.dumps(actual, separators=(',', ':')) != json.dumps(exp, separators=(',', ':')):
                status = "Wrong Answer"
            
        if extracted_expected[0] is not None:
            statuses.append({"status": status, "expected": extracted_expected[0]})
        else:
            statuses.append(status)
        
    print("SF_EVAL_START")
    print(json.dumps(statuses))
    print("SF_EVAL_END")

evaluate()
)python");

    evaluatorCode.replace("_SF_DATA_JSON_HERE_", escapedData);

    QFile dumpFile("/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/dump_evaluator.py");
    if (dumpFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        dumpFile.write(evaluatorCode.toUtf8());
        dumpFile.close();
    }

    EmbeddedRunner::Result r = m_pythonRunner->execute(evaluatorCode, "", &m_stopRequested);
    
    if (r.exitCode == 0) {
        int startIdx = r.output.indexOf("SF_EVAL_START");
        int endIdx = r.output.indexOf("SF_EVAL_END");
        if (startIdx != -1 && endIdx != -1) {
            startIdx += QString("SF_EVAL_START").length();
            QString jsonStr = r.output.mid(startIdx, endIdx - startIdx).trimmed();
            QJsonParseError parseError;
            QJsonDocument doc = QJsonDocument::fromJson(jsonStr.toUtf8(), &parseError);
            if (parseError.error == QJsonParseError::NoError) {
                return doc.array();
            }
        }
    } else {
        qDebug() << "Python evaluation failed!" << r.error << r.output;
    }
    
    // Fallback if it fails
    QJsonArray fallback;
    for (int i = 0; i < results.size(); ++i) fallback.append("System Error");
    return fallback;
}

int CodeRunner::calculateSmartTimeLimit(const QString &languageId) {
    int timeLimit = 60000; // default 60s
    bool isSlowLang = (languageId == "python" || languageId == "javascript" || languageId == "js");
    
    QJsonArray tags = m_currentProblem["tags"].toArray();
    bool hasBacktracking = false;
    for (int i = 0; i < tags.size(); ++i) {
        QString tag = tags[i].toString().toLower();
        if (tag.contains("backtracking") || tag.contains("recursion") || tag.contains("dynamic programming")) {
            hasBacktracking = true;
            break;
        }
    }
    
    bool smallConstraints = false;
    QJsonArray constraints = m_currentProblem["constraints"].toArray();
    for (int i = 0; i < constraints.size(); ++i) {
        QString c = constraints[i].toString();
        QRegularExpression regex(R"(<=\s*(\d+))");
        QRegularExpressionMatch match = regex.match(c);
        if (match.hasMatch()) {
            int val = match.captured(1).toInt();
            if (val <= 30) {
                smallConstraints = true;
                break;
            }
        }
    }
    
    if (hasBacktracking || smallConstraints) {
        timeLimit = isSlowLang ? 180000 : 120000; // 3 mins for slow, 2 mins for fast
    }
    
    int numTests = m_currentProblem["testCases"].toArray().size();
    if (numTests > 0) {
        int minTime = numTests * (isSlowLang ? 2000 : 500);
        if (timeLimit < minTime) {
            timeLimit = minTime;
        }
    }
    
    return timeLimit;
}

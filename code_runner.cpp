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
}

CodeRunner::~CodeRunner() {
    delete m_pythonRunner;
    delete m_wasmRunnerC;
    delete m_wasmRunnerCpp;
}

EmbeddedRunner* CodeRunner::getRunner(const QString &languageId) {
    if (languageId == "python") return m_pythonRunner;
    if (languageId == "c") return m_wasmRunnerC;
    if (languageId == "cpp" || languageId == "c++") return m_wasmRunnerCpp;
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
    } else {
        QString fullCode = code;
        if (schema.isValid()) {
            fullCode += DriverGenerator::generateDriver(schema, languageId);
        }

        for (int i = 0; i < tests.size() && !m_stopRequested; ++i) {
            emit progress(i + 1, tests.size());
            executeTestRaw(runner, fullCode, tests[i].toObject(), i);
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
    } else {
        QString fullCode = code;
        if (schema.isValid()) {
            fullCode += DriverGenerator::generateDriver(schema, languageId);
        }

        executeTestRaw(runner, fullCode, tests[testIndex].toObject(), testIndex);
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
                                int index)
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
        actual = r.error.trimmed();
        if (actual.isEmpty()) {
            actual = QString("Process exited with code %1").arg(r.exitCode);
            if (!r.error.isEmpty()) {
                actual += "\nError: " + r.error;
            } else if (r.exitCode == -1 && runner->languageId() == "c") {
                actual += "\n(Check if 'main' function is defined, or if there was a relocation error)";
            }
        }
    } else if (r.timedOut) {
        status = "Time Limit Exceeded";
    } else {
        // BUG-10: surface compile/parse errors for any language, not just C.
        // Heuristic: non-zero exit with no stdout and non-empty stderr = compile error.
        if (r.exitCode != 0 && actual.isEmpty() && !r.error.isEmpty()) {
            emit compilationError(r.error);
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
    
    QString fullExecutionCode = harnessTemplate
        .arg(code)
        .arg(QLatin1String("\"") + escapedManifest + QLatin1String("\""));
        
    EmbeddedRunner::Result r = m_pythonRunner->execute(fullExecutionCode, "", &m_stopRequested);
    
    if (r.exitCode != 0 && r.output.isEmpty()) {
        emit compilationError(r.error.isEmpty() ? "Execution failed with non-zero exit code" : r.error);
        return;
    }
    
    QString output = r.output;
    int startIdx = output.indexOf("SF_JSON_SUMMARY_START");
    int endIdx = output.indexOf("SF_JSON_SUMMARY_END");
    
    if (startIdx == -1 || endIdx == -1) {
        emit compilationError("Harness output did not contain test summary. Output:\n" + output + "\nError:\n" + r.error);
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

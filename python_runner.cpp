// python_runner.cpp  —  PythonRunner implementation
// ─────────────────────────────────────────────────────────────────────────────
// The CPython C API lets us replace sys.stdout / sys.stderr / sys.stdin with
// io.StringIO objects before each run.  After execution we call getvalue()
// to retrieve the captured text and restore the originals.
//
// PyRun_String() is used instead of PyRun_SimpleString() so we can distinguish
// compile-time SyntaxError from runtime exceptions.
// ─────────────────────────────────────────────────────────────────────────────

#include "python_runner.h"

// Python.h must come first — it redefines some POSIX macros
#pragma push_macro("slots")
#undef slots
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#pragma pop_macro("slots")

#include <QMutexLocker>
#include <QDebug>

// ── RAII helper ──────────────────────────────────────────────────────────────
// Automatically calls Py_XDECREF on destruction.
struct PyObj {
    PyObject *o;
    explicit PyObj(PyObject *p = nullptr) : o(p) {}
    ~PyObj() { Py_XDECREF(o); }
    operator PyObject*() const { return o; }
    bool ok() const { return o != nullptr; }
    PyObject *release() { PyObject *r = o; o = nullptr; return r; }
};

// ── Format the current Python exception as a QString ────────────────────────
static QString format_py_exception()
{
    PyObject *ptype = nullptr, *pval = nullptr, *ptb = nullptr;
    PyErr_Fetch(&ptype, &pval, &ptb);
    PyErr_NormalizeException(&ptype, &pval, &ptb);

    QString result;

    // Get traceback string via traceback module
    if (ptb) {
        PyObj tbMod(PyImport_ImportModule("traceback"));
        if (tbMod.ok()) {
            PyObj fmtFn(PyObject_GetAttrString(tbMod, "format_exception"));
            if (fmtFn.ok()) {
                PyObj args(PyTuple_Pack(3, ptype, pval, ptb));
                PyObj lines(PyObject_CallObject(fmtFn, args));
                if (lines.ok()) {
                    PyObj joined(PyUnicode_Join(
                        PyUnicode_FromString(""), lines));
                    if (joined.ok()) {
                        const char *s = PyUnicode_AsUTF8(joined);
                        if (s) result = QString::fromUtf8(s);
                    }
                }
            }
        }
    }

    // Fallback: just stringify the value
    if (result.isEmpty() && pval) {
        PyObj s(PyObject_Str(pval));
        if (s.ok()) {
            const char *cs = PyUnicode_AsUTF8(s);
            if (cs) result = QString::fromUtf8(cs);
        }
    }

    Py_XDECREF(ptype);
    Py_XDECREF(pval);
    Py_XDECREF(ptb);
    return result.isEmpty() ? QStringLiteral("Unknown Python error") : result;
}

// ── Get the text from an io.StringIO object ──────────────────────────────────
static QString stringio_getvalue(PyObject *sio)
{
    if (!sio) return {};
    PyObj val(PyObject_CallMethod(sio, "getvalue", nullptr));
    if (!val.ok()) return {};
    const char *s = PyUnicode_AsUTF8(val);
    return s ? QString::fromUtf8(s) : QString{};
}

// ─────────────────────────────────────────────────────────────────────────────
PythonRunner::PythonRunner()
{
    if (Py_IsInitialized()) {
        // Another PythonRunner already initialised the interpreter.
        // This is fine — we share it.
        m_initialised = true;
        return;
    }

    // Prevent Python from installing its own signal handlers (SIGINT etc.)
    // — we don't want them interfering with Qt.
    Py_InitializeEx(0);

    if (!Py_IsInitialized()) {
        qWarning() << "PythonRunner: Py_Initialize() failed";
        return;
    }
    m_initialised = true;
    qDebug() << "PythonRunner: CPython" << Py_GetVersion() << "initialised";
}

PythonRunner::~PythonRunner()
{
    // Only finalise if we own the interpreter (first instance).
    // In practice, do NOT call Py_Finalize() if Qt plugins or other
    // code might still hold PyObjects — it can crash.
    // Safe to leave the interpreter alive until process exit.
}

// ─────────────────────────────────────────────────────────────────────────────
EmbeddedRunner::Result PythonRunner::execute(const QString &code,
                                              const QString &stdinInput,
                                              volatile bool *)
{
    QMutexLocker lock(&m_mutex);
    Result result;

    if (!m_initialised) {
        result.error    = QStringLiteral("[PythonRunner] CPython not initialised");
        result.exitCode = -1;
        return result;
    }

    // ── 1. Import io + sys ───────────────────────────────────────────────────
    PyObj ioMod (PyImport_ImportModule("io"));
    PyObj sysMod(PyImport_ImportModule("sys"));
    if (!ioMod.ok() || !sysMod.ok()) {
        result.error = QStringLiteral("Failed to import io / sys");
        result.exitCode = -1;
        return result;
    }

    // ── 2. Save real stdout / stderr / stdin ─────────────────────────────────
    PyObj realStdout(PyObject_GetAttrString(sysMod, "stdout"));
    PyObj realStderr(PyObject_GetAttrString(sysMod, "stderr"));
    PyObj realStdin (PyObject_GetAttrString(sysMod, "stdin"));

    // ── 3. Create StringIO captures ──────────────────────────────────────────
    PyObj captureOut(PyObject_CallMethod(ioMod, "StringIO", nullptr));
    PyObj captureErr(PyObject_CallMethod(ioMod, "StringIO", nullptr));

    // stdin: StringIO(stdinInput)
    QByteArray inputUtf8 = stdinInput.toUtf8();
    PyObj pyInput(PyUnicode_FromStringAndSize(inputUtf8.constData(),
                                               inputUtf8.size()));
    PyObj captureIn(PyObject_CallMethod(ioMod, "StringIO", "O",
                                         static_cast<PyObject*>(pyInput)));

    if (!captureOut.ok() || !captureErr.ok() || !captureIn.ok()) {
        result.error = QStringLiteral("Failed to create StringIO objects");
        result.exitCode = -1;
        return result;
    }

    // ── 4. Replace sys.stdout / stderr / stdin ───────────────────────────────
    PyObject_SetAttrString(sysMod, "stdout", captureOut);
    PyObject_SetAttrString(sysMod, "stderr", captureErr);
    PyObject_SetAttrString(sysMod, "stdin",  captureIn);

    // ── 5. Build a clean __main__ module globals dict ─────────────────────────
    PyObj mainMod (PyImport_AddModule("__main__"));   // borrowed ref — don't decref
    PyObject *globals = PyModule_GetDict(mainMod);    // borrowed
    // Add builtins so the user code has access to print, input, etc.
    PyObj builtinsMod(PyImport_ImportModule("builtins"));
    PyDict_SetItemString(globals, "__builtins__", builtinsMod);

    // ── 6. Execute ──────────────────────────────────────────────────────────
    QByteArray src = code.toUtf8();
    PyObject *ret = PyRun_String(src.constData(),
                                  Py_file_input,   // compile as a module (allows top-level statements)
                                  globals,
                                  globals);

    if (!ret) {
        // There was an exception
        result.error    = format_py_exception();
        result.exitCode = 1;
        PyErr_Clear();
    } else {
        Py_DECREF(ret);
        result.exitCode = 0;
    }

    // ── 7. Flush + collect captured output ────────────────────────────────────
    // Flush in case the user code used print(..., end="") without a newline
    PyObject_CallMethod(captureOut, "flush", nullptr);
    PyObject_CallMethod(captureErr, "flush", nullptr);

    result.output = stringio_getvalue(captureOut);
    if (!result.error.isEmpty()) {
        // Append anything that went to stderr (e.g. warnings)
        QString errStream = stringio_getvalue(captureErr);
        if (!errStream.isEmpty())
            result.error = errStream + result.error;
    } else {
        result.error = stringio_getvalue(captureErr);
    }

    // ── 8. Restore sys.stdout / stderr / stdin ────────────────────────────────
    PyObject_SetAttrString(sysMod, "stdout", realStdout);
    PyObject_SetAttrString(sysMod, "stderr", realStderr);
    PyObject_SetAttrString(sysMod, "stdin",  realStdin);

    return result;
}

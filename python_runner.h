#pragma once
#include "embedded_runner.h"
#include <QMutex>

// ─────────────────────────────────────────────────────────────────────────────
// PythonRunner  —  runs Python 3 in-process via CPython's embedding API.
//
// Output capture:
//   sys.stdout and sys.stderr are replaced with io.StringIO() objects before
//   each execution.  After PyRun_String() returns, getvalue() retrieves the
//   captured text.  The real sys.stdout/stderr are restored afterwards.
//
// Stdin injection:
//   sys.stdin is replaced with io.StringIO(stdinInput) so input() / sys.stdin
//   reads work correctly without a real terminal.
//
// Interpreter lifecycle:
//   Py_Initialize() is called ONCE in the constructor (it is expensive and
//   not meant to be called repeatedly).  The interpreter stays alive for the
//   lifetime of the PythonRunner object.
//
//   IMPORTANT: only create ONE PythonRunner per process.  CPython is not
//   designed for multiple simultaneous interpreters without sub-interpreters.
//
// Thread safety:
//   m_mutex serialises all Python calls.  CPython's GIL does not protect you
//   from calling into the interpreter from multiple Qt threads simultaneously
//   without acquiring the GIL first.
// ─────────────────────────────────────────────────────────────────────────────

class PythonRunner : public EmbeddedRunner
{
public:
    // Initialises the CPython interpreter.  Call once per process.
    explicit PythonRunner();

    // Finalises the interpreter.  Do not create another after this.
    ~PythonRunner() override;

    Result execute(const QString &code,
                   const QString &stdinInput,
                   volatile bool *abort = nullptr,
                   const QMap<QString, QString> &additionalFiles = {}) override;

    QString languageId()   const override { return QStringLiteral("python"); }
    QString languageName() const override { return QStringLiteral("Python");  }

    bool isInitialised() const { return m_initialised; }

private:
    bool    m_initialised = false;
    QMutex  m_mutex;
};

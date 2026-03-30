#pragma once
#include "embedded_runner.h"
#include <QMutex>

// ─────────────────────────────────────────────────────────────────────────────
// TccRunner  —  compiles and runs C code in-process via libtcc.
//
// Key improvements over the old dead-code `Compiler` class:
//   • Captures stdout/stderr via pipe+dup2  (output goes to UI, not terminal)
//   • Injects stdin via pipe+dup2           (scanf / fgets work with test input)
//   • Intercepts exit() via longjmp         (exit(0) won't kill the Qt process)
//   • Captures TCC compile errors into result.error
//   • QMutex-protected (dup2 modifies global file descriptors)
// ─────────────────────────────────────────────────────────────────────────────

class TccRunner : public EmbeddedRunner
{
public:
    explicit TccRunner();
    ~TccRunner() override = default;

    Result execute(const QString &code,
                   const QString &stdinInput,
                   volatile bool *abort = nullptr) override;

    QString languageId()   const override { return QStringLiteral("c"); }
    QString languageName() const override { return QStringLiteral("C");  }

    // Path to tcc runtime files (libtcc1.a, include/ etc.).
    // Defaults to <appDir>/tcc — override if needed.
    void setLibPath(const QString &path) { m_libPath = path; }

private:
    QString m_libPath;
    QMutex  m_mutex;   // serialise dup2 — one TCC run at a time
};

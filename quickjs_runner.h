#pragma once
#include "embedded_runner.h"
#include <QMutex>

// ─────────────────────────────────────────────────────────────────────────────
// QuickJsRunner  —  runs JavaScript in-process via QuickJS (Fabrice Bellard).
//
// Output capture:
//   console.log() / print() are overridden with C functions that append
//   to an output buffer — no pipe tricks needed.
//
// Stdin injection:
//   readline() is overridden to pop lines from the provided stdinInput string.
//
// Error handling:
//   JS exceptions and syntax errors are caught and placed in result.error.
//
// Thread safety:
//   Each execute() call creates a fresh JSRuntime + JSContext and destroys
//   them on exit, so concurrent calls are safe with separate TccRunner
//   instances.  If you share one QuickJsRunner across threads, guard with
//   the internal m_mutex.
// ─────────────────────────────────────────────────────────────────────────────

class QuickJsRunner : public EmbeddedRunner
{
public:
    explicit QuickJsRunner();
    ~QuickJsRunner() override = default;

    Result execute(const QString &code,
                   const QString &stdinInput,
                   volatile bool *abort = nullptr) override;

    QString languageId()   const override { return QStringLiteral("javascript"); }
    QString languageName() const override { return QStringLiteral("JavaScript");  }

private:
    QMutex m_mutex;
};

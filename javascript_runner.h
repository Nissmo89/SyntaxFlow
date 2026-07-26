#pragma once
#include "embedded_runner.h"
#include <QMutex>

// ─────────────────────────────────────────────────────────────────────────────
// JavascriptRunner  —  runs Javascript in-process.
//
// Output capture:
//   Output is captured and returned in the Result object.
//
// Stdin injection:
//   Stdin is handled via provided input string.
//
// Interpreter lifecycle:
//   The interpreter stays alive for the lifetime of the JavascriptRunner object.
//
//   IMPORTANT: only create ONE JavascriptRunner per process.
//
// Thread safety:
//   m_mutex serialises all calls.
// ─────────────────────────────────────────────────────────────────────────────

class JavascriptRunner : public EmbeddedRunner
{
public:
    // Initialises the interpreter.  Call once per process.
    explicit JavascriptRunner();

    // Finalises the interpreter.  Do not create another after this.
    ~JavascriptRunner() override;

    Result execute(const QString &code,
                   const QString &stdinInput = "",
                   volatile bool *abort = nullptr,
                   const QMap<QString, QString> &additionalFiles = {}) override;

    QString languageId()   const override { return QStringLiteral("javascript"); }
    QString languageName() const override { return QStringLiteral("Javascript");  }

    bool isInitialised() const { return m_initialised; }

private:
    bool    m_initialised = false;
    QMutex  m_mutex;
};

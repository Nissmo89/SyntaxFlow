// clang_runner.h  —  macOS QProcess-based C/C++ runner
// ─────────────────────────────────────────────────────────────────────────────
// Used only when SF_USE_CLANG_RUNNER is defined (Apple platforms).
// TCC does not support macOS / Apple Silicon, so we invoke system clang/clang++
// via QProcess: write source to a temp file, compile to a temp binary, run it,
// and capture stdout / stderr.
// ─────────────────────────────────────────────────────────────────────────────
#pragma once
#ifdef SF_USE_CLANG_RUNNER

#include "embedded_runner.h"
#include <QMutex>
#include <QString>

class ClangRunner : public EmbeddedRunner
{
public:
    explicit ClangRunner(bool isCpp = false);

    Result execute(const QString &code,
                   const QString &stdinInput,
                   volatile bool *stopRequested) override;

    QString languageId() const override { return m_isCpp ? QStringLiteral("cpp") : QStringLiteral("c"); }

private:
    bool    m_isCpp;
    QMutex  m_mutex;
};

#endif // SF_USE_CLANG_RUNNER

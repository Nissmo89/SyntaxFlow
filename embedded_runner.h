#pragma once
#include <QString>

// ─────────────────────────────────────────────────────────────────────────────
// EmbeddedRunner  —  base class every in-process language runner implements.
//
// Usage:
//   EmbeddedRunner::Result r = runner->execute(code, stdinInput, &m_stopRequested);
//   // r.output  → captured stdout
//   // r.error   → captured stderr / compile errors
//   // r.exitCode / r.timedOut
// ─────────────────────────────────────────────────────────────────────────────

class EmbeddedRunner
{
public:
    struct Result {
        QString output;
        QString error;
        int     exitCode = 0;
        bool    timedOut = false;
    };

    virtual ~EmbeddedRunner() = default;

    // Compile + run (or just interpret) code with provided stdin input.
    // Set *abort = true from another thread to request a stop (best-effort).
    virtual Result execute(const QString &code,
                           const QString &stdinInput,
                           volatile bool *abort = nullptr) = 0;

    virtual QString languageId()   const = 0;   // e.g. "c", "javascript", "python"
    virtual QString languageName() const = 0;   // e.g. "C",  "JavaScript",  "Python"
};

#pragma once
#include "embedded_runner.h"
#include <QMutex>

// ─────────────────────────────────────────────────────────────────────────────
// WasmRunner  —  WASM-based C/C++ cross-platform runner
//
// Compiles the given source via Wasmer's clang/clang WASM package,
// runs the resulting binary in Wasmer, and captures stdout/stderr.
// ─────────────────────────────────────────────────────────────────────────────

class WasmRunner : public EmbeddedRunner
{
public:
    WasmRunner(bool isCpp = false);

    Result execute(const QString &code,
                   const QString &stdinInput,
                   volatile bool *abort = nullptr) override;

    QString languageId()   const override { return m_isCpp ? "cpp" : "c"; }
    QString languageName() const override { return m_isCpp ? "C++" : "C"; }

private:
    bool m_isCpp;
    QMutex m_mutex;
    QString getWasmerExecutable() const;
};

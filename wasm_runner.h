#pragma once
#include "embedded_runner.h"

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
                   volatile bool *abort = nullptr,
                   const QMap<QString, QString> &additionalFiles = {},
                   int timeoutMs = 60000) override;

    QString languageId()   const override { return m_isCpp ? "cpp" : "c"; }
    QString languageName() const override { return m_isCpp ? "C++" : "C"; }

private:
    bool m_isCpp;
    QString getWasmerExecutable() const;
    QString getWasiSdkCompiler() const;
};

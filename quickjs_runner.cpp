// quickjs_runner.cpp  —  QuickJsRunner implementation
// ─────────────────────────────────────────────────────────────────────────────
// How output capture works:
//   QuickJS lets you define any JS global via JS_NewCFunction().
//   We override `console.log`, `print`, and `readline` with C++ lambdas
//   (wrapped in plain C functions) that read from / write to simple buffers
//   stored in the JSRuntime opaque pointer.
//
//   No pipe tricks, no dup2, no process redirection needed.
// ─────────────────────────────────────────────────────────────────────────────

#include "quickjs_runner.h"
#include <quickjs.h>       // from libs/quickjs/
#include <QMutexLocker>
#include <QStringList>

// ── Per-execution context stored in JSRuntime opaque ────────────────────────
struct QjsCtx {
    QString         output;
    QString         error;
    QStringList     stdinLines;
    int             stdinPos = 0;
};

// ── C functions registered as JS globals ────────────────────────────────────

// Shared implementation for console.log / print
static JSValue js_print(JSContext *ctx, JSValueConst /*this_val*/,
                         int argc, JSValueConst *argv)
{
    auto *qc = static_cast<QjsCtx*>(JS_GetRuntimeOpaque(JS_GetRuntime(ctx)));
    for (int i = 0; i < argc; ++i) {
        if (i > 0) qc->output += QLatin1Char(' ');
        const char *str = JS_ToCString(ctx, argv[i]);
        if (str) {
            qc->output += QString::fromUtf8(str);
            JS_FreeCString(ctx, str);
        }
    }
    qc->output += QLatin1Char('\n');
    return JS_UNDEFINED;
}

// readline() → returns next line from the injected stdin string
static JSValue js_readline(JSContext *ctx, JSValueConst /*this_val*/,
                             int /*argc*/, JSValueConst * /*argv*/)
{
    auto *qc = static_cast<QjsCtx*>(JS_GetRuntimeOpaque(JS_GetRuntime(ctx)));
    if (qc->stdinPos >= qc->stdinLines.size())
        return JS_NULL;   // EOF
    const QString &line = qc->stdinLines.at(qc->stdinPos++);
    return JS_NewStringLen(ctx, line.toUtf8().constData(),
                           static_cast<size_t>(line.toUtf8().size()));
}

// console.error / console.warn → captured into result.error
static JSValue js_console_error(JSContext *ctx, JSValueConst /*this_val*/,
                                  int argc, JSValueConst *argv)
{
    auto *qc = static_cast<QjsCtx*>(JS_GetRuntimeOpaque(JS_GetRuntime(ctx)));
    for (int i = 0; i < argc; ++i) {
        if (i > 0) qc->error += QLatin1Char(' ');
        const char *str = JS_ToCString(ctx, argv[i]);
        if (str) {
            qc->error += QString::fromUtf8(str);
            JS_FreeCString(ctx, str);
        }
    }
    qc->error += QLatin1Char('\n');
    return JS_UNDEFINED;
}

// ── Helper: format a pending JS exception into a QString ─────────────────────
static QString format_exception(JSContext *ctx)
{
    JSValue exc = JS_GetException(ctx);
    QString msg;

    JSValue msgVal = JS_GetPropertyStr(ctx, exc, "message");
    if (!JS_IsUndefined(msgVal)) {
        const char *s = JS_ToCString(ctx, msgVal);
        if (s) { msg += QString::fromUtf8(s); JS_FreeCString(ctx, s); }
    }
    JS_FreeValue(ctx, msgVal);

    // Stack trace
    JSValue stack = JS_GetPropertyStr(ctx, exc, "stack");
    if (!JS_IsUndefined(stack)) {
        const char *s = JS_ToCString(ctx, stack);
        if (s) { msg += QLatin1Char('\n'); msg += QString::fromUtf8(s); JS_FreeCString(ctx, s); }
    }
    JS_FreeValue(ctx, stack);
    JS_FreeValue(ctx, exc);
    return msg;
}

// ─────────────────────────────────────────────────────────────────────────────
QuickJsRunner::QuickJsRunner() {}

EmbeddedRunner::Result QuickJsRunner::execute(const QString &code,
                                               const QString &stdinInput,
                                               volatile bool *)
{
    QMutexLocker lock(&m_mutex);
    Result result;

    // 1. Create runtime + context
    JSRuntime *rt = JS_NewRuntime();
    // Set a memory limit (32 MB) — stops infinite allocation
    JS_SetMemoryLimit(rt, 32 * 1024 * 1024);
    // Set a stack size limit (1 MB) — stops infinite recursion crashing the app
    JS_SetMaxStackSize(rt, 1 * 1024 * 1024);

    QjsCtx qc;
    // Split stdin into lines for readline()
    if (!stdinInput.isEmpty())
        qc.stdinLines = stdinInput.split(QLatin1Char('\n'), Qt::SkipEmptyParts);

    JS_SetRuntimeOpaque(rt, &qc);

    JSContext *ctx = JS_NewContext(rt);

    // 2. Register globals
    JSValue global = JS_GetGlobalObject(ctx);

    // print() — top-level function, used in many competitive programming solutions
    JS_SetPropertyStr(ctx, global, "print",
                      JS_NewCFunction(ctx, js_print, "print", 1));

    // readline() — top-level, same as Node.js readline in competitive scenarios
    JS_SetPropertyStr(ctx, global, "readline",
                      JS_NewCFunction(ctx, js_readline, "readline", 0));

    // console object with .log / .error / .warn
    JSValue console = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, console, "log",
                      JS_NewCFunction(ctx, js_print, "log", 1));
    JS_SetPropertyStr(ctx, console, "info",
                      JS_NewCFunction(ctx, js_print, "info", 1));
    JS_SetPropertyStr(ctx, console, "warn",
                      JS_NewCFunction(ctx, js_console_error, "warn", 1));
    JS_SetPropertyStr(ctx, console, "error",
                      JS_NewCFunction(ctx, js_console_error, "error", 1));
    JS_SetPropertyStr(ctx, global, "console", console);

    JS_FreeValue(ctx, global);

    // 3. Evaluate
    QByteArray src = code.toUtf8();
    JSValue val = JS_Eval(ctx, src.constData(), static_cast<size_t>(src.size()),
                          "<input>", JS_EVAL_TYPE_GLOBAL);

    if (JS_IsException(val)) {
        result.error    = format_exception(ctx);
        result.exitCode = 1;
    } else {
        result.exitCode = 0;
    }
    JS_FreeValue(ctx, val);

    // 4. Collect output
    result.output = qc.output;
    if (!qc.error.isEmpty())
        result.error = qc.error + result.error;

    JS_FreeContext(ctx);
    JS_FreeRuntime(rt);
    return result;
}

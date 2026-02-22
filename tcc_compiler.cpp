#include "tcc_compiler.h"
#include "qdir.h"
#include <libtcc.h>
#include <cstdio>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <QCoreApplication>

Compiler::Compiler(QObject *parent)
    : QObject(parent)
{
}

bool Compiler::compileAndRun(const QString &code,
                             std::function<void(const QString &)> outputCallback,
                             std::function<void(const QString &)> errorCallback)
{
    TCCState *s = tcc_new();
    if (!s) {
        errorCallback("Failed to create TCC state");
        return false;
    }

    QString basePath = QCoreApplication::applicationDirPath() + "/tcc";
    tcc_set_lib_path(s, basePath.toUtf8().constData());

    tcc_set_output_type(s, TCC_OUTPUT_MEMORY);

    tcc_set_error_func(s, nullptr,
                       [](void*, const char* msg){
                           fprintf(stderr, "TCC ERROR: %s\n", msg);
                       });

    if (tcc_compile_string(s, code.toUtf8().constData()) < 0) {
        errorCallback("Compilation failed");
        tcc_delete(s);
        return false;
    }

    // 🔥 FIX: replace TCC_RELOCATE_AUTO with (void*)1
    if (tcc_relocate(s) < 0) {
        errorCallback("Relocation failed");
        tcc_delete(s);
        return false;
    }


    int ret = tcc_run(s, 0, nullptr);

    outputCallback(QString("Program exited with code %1").arg(ret));

    tcc_delete(s);
    emit finished();
    return true;
}

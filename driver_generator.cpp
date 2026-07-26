#include "driver_generator.h"

MethodSchema MethodSchema::fromJson(const QJsonObject& obj) {
    MethodSchema schema;
    schema.name = obj["name"].toString();
    schema.returnType = obj["returnType"].toString();
    QJsonArray paramsArray = obj["params"].toArray();
    for (int i = 0; i < paramsArray.size(); ++i) {
        QJsonObject pObj = paramsArray[i].toObject();
        schema.params.append({pObj["name"].toString(), pObj["type"].toString()});
    }
    return schema;
}

// Map schema types to C types
static QString toCType(const QString& type) {
    if (type == "int")    return "int";
    if (type == "double") return "double";
    if (type == "string") return "char*";
    if (type.startsWith("vector<")) return "int*"; // int arrays via int*
    return "int";
}

QString DriverGenerator::generateTemplate(const MethodSchema& schema, const QString& languageId) {
    QString lang = languageId.toLower();
    
    if (lang == "c" || lang == "cpp") {
        // Pure C99 — TCC is a C-only compiler. No classes, no std::, no templates.
        QString paramsStr;
        for (int i = 0; i < schema.params.size(); ++i) {
            if (i > 0) paramsStr += ", ";
            const auto& p = schema.params[i];
            if (p.type.startsWith("vector<")) {
                paramsStr += "int* " + p.name + ", int " + p.name + "_size";
            } else {
                paramsStr += toCType(p.type) + " " + p.name;
            }
        }

        QString tpl = "#include <stdio.h>\n#include <stdlib.h>\n\n";
        tpl += toCType(schema.returnType) + " " + schema.name + "(" + paramsStr + ") {\n";
        tpl += "    /* Your code here */\n";
        if (schema.returnType == "int") tpl += "    return 0;\n";
        else if (schema.returnType == "double") tpl += "    return 0.0;\n";
        tpl += "}\n";
        return tpl;
    }
    else if (lang == "python") {
        QString paramsStr = "self";
        for (const auto& p : schema.params) {
            QString type = p.type;
            if (type == "int") type = "int";
            else if (type == "vector<int>") type = "List[int]";
            else if (type == "string") type = "str";
            else if (type == "vector<string>") type = "List[str]";
            paramsStr += ", " + p.name + ": " + type;
        }
        
        QString pyReturn = schema.returnType;
        if (pyReturn == "vector<int>") pyReturn = "List[int]";
        else if (pyReturn == "vector<string>") pyReturn = "List[str]";
        
        QString tpl = "from typing import List\n\nclass Solution:\n";
        tpl += "    def " + schema.name + "(" + paramsStr + ") -> " + pyReturn + ":\n";
        tpl += "        # Your code here\n";
        tpl += "        pass\n";
        return tpl;
    }
    else if (lang == "javascript" || lang == "js") {
        QString paramsStr;
        for (int i = 0; i < schema.params.size(); ++i) {
            if (i > 0) paramsStr += ", ";
            paramsStr += schema.params[i].name;
        }
        
        QString tpl = "/**\n";
        for (const auto& p : schema.params) {
            tpl += " * @param {" + p.type + "} " + p.name + "\n";
        }
        tpl += " * @return {" + schema.returnType + "}\n";
        tpl += " */\n";
        tpl += "var " + schema.name + " = function(" + paramsStr + ") {\n";
        tpl += "    // Your code here\n";
        tpl += "\n};\n";
        return tpl;
    }
    
    return "// Unsupported language template\n";
}

QString DriverGenerator::generateDriver(const MethodSchema& schema, const QString& languageId) {
    if (!schema.isValid()) return "";
    
    QString lang = languageId.toLower();
    if (lang == "c" || lang == "cpp") return generateCppDriver(schema);
    if (lang == "python") return generatePythonDriver(schema);
    if (lang == "javascript" || lang == "js") return generateJsDriver(schema);
    
    return "";
}

QString DriverGenerator::generateCppDriver(const MethodSchema& schema) {
    // Pure C99 driver — TCC is a C compiler only.
    // Entry point is _sf_main (not main) so TccRunner can use tcc_get_symbol
    // without triggering tcc_run()'s internal libtcc1.a loading crash.
    QString out = "\n/* --- SYNTAXFLOW DRIVER --- */\n";
    out += "int main() {\n";
    
    QString callArgs;
    for (int i = 0; i < schema.params.size(); ++i) {
        const auto& p = schema.params[i];
        if (i > 0) callArgs += ", ";
        
        if (p.type == "int") {
            out += "    int _" + p.name + ";\n";
            out += "    scanf(\"%d\", &_" + p.name + ");\n";
            callArgs += "_" + p.name;
        } else if (p.type == "double") {
            out += "    double _" + p.name + ";\n";
            out += "    scanf(\"%lf\", &_" + p.name + ");\n";
            callArgs += "_" + p.name;
        } else if (p.type.startsWith("vector<")) {
            out += "    int _size_" + p.name + ";\n";
            out += "    scanf(\"%d\", &_size_" + p.name + ");\n";
            out += "    int* _" + p.name + " = (int*)malloc(_size_" + p.name + " * sizeof(int));\n";
            out += "    for(int i=0; i<_size_" + p.name + "; ++i) scanf(\"%d\", &_" + p.name + "[i]);\n";
            callArgs += "_" + p.name + ", _size_" + p.name;
        }
    }
    
    if (schema.returnType == "void") {
        out += "    " + schema.name + "(" + callArgs + ");\n";
    } else if (schema.returnType == "int") {
        out += "    int _result = " + schema.name + "(" + callArgs + ");\n";
        out += "    printf(\"%d\\n\", _result);\n";
    } else if (schema.returnType == "double") {
        out += "    double _result = " + schema.name + "(" + callArgs + ");\n";
        out += "    printf(\"%g\\n\", _result);\n";
    } else if (schema.returnType.startsWith("vector<")) {
        out += "    int _result_size = 0;\n";
        out += "    int* _result = " + schema.name + "(" + callArgs + ", &_result_size);\n";
        out += "    for(int i=0; i<_result_size; ++i) {\n";
        out += "        if(i>0) printf(\" \");\n";
        out += "        printf(\"%d\", _result[i]);\n";
        out += "    }\n";
        out += "    printf(\"\\n\");\n";
    }
    out += "    return 0;\n}\n";
    return out;
}

QString DriverGenerator::generatePythonDriver(const MethodSchema& schema) {
    QString out = "\n# --- SYNTAXFLOW DRIVER WRAPPER ---\n";
    out += "if __name__ == '__main__':\n";
    out += "    import sys\n";
    out += "    _tokens = sys.stdin.read().split()\n";
    out += "    _tok_idx = 0\n";
    out += "    def _next_token():\n";
    out += "        global _tok_idx\n";
    out += "        if _tok_idx >= len(_tokens): return ''\n";
    out += "        res = _tokens[_tok_idx]\n";
    out += "        _tok_idx += 1\n";
    out += "        return res\n\n";

    QString callArgs;
    for (int i = 0; i < schema.params.size(); ++i) {
        const auto& p = schema.params[i];
        if (i > 0) callArgs += ", ";
        callArgs += "_" + p.name;
        
        if (p.type == "int") {
            out += "    _" + p.name + " = int(_next_token())\n";
        } else if (p.type == "double") {
            out += "    _" + p.name + " = float(_next_token())\n";
        } else if (p.type == "string") {
            out += "    _" + p.name + " = _next_token()\n";
        } else if (p.type.startsWith("vector<")) {
            QString baseTypeStr = p.type.mid(7, p.type.length() - 8);
            QString castFunc = (baseTypeStr == "int") ? "int" : ((baseTypeStr == "double") ? "float" : "str");
            out += "    _size_" + p.name + " = int(_next_token())\n";
            out += "    _" + p.name + " = [" + castFunc + "(_next_token()) for _ in range(_size_" + p.name + ")]\n";
        }
    }
    
    out += "    _sol = Solution()\n";
    
    if (schema.returnType == "void") {
        out += "    _sol." + schema.name + "(" + callArgs + ")\n";
    } else {
        out += "    _result = _sol." + schema.name + "(" + callArgs + ")\n";
        
        if (schema.returnType == "int" || schema.returnType == "double" || schema.returnType == "string") {
            out += "    print(_result)\n";
        } else if (schema.returnType.startsWith("vector<")) {
            out += "    print(' '.join(map(str, _result)))\n";
        }
    }
    return out;
}

QString DriverGenerator::generateJsDriver(const MethodSchema& schema) {
    QString out = "\n// --- SYNTAXFLOW DRIVER WRAPPER ---\n";
    out += "(function() {\n";
    out += "    let _inputStr = '';\n";
    out += "    while (true) {\n";
    out += "        let t = readline();\n";
    out += "        if (t === null) break;\n";
    out += "        _inputStr += t + ' ';\n";
    out += "    }\n";
    out += "    let _tokens = _inputStr.trim().split(/\\s+/);\n";
    out += "    let _tok_idx = 0;\n";
    out += "    function _nextToken() { return _tok_idx < _tokens.length ? _tokens[_tok_idx++] : ''; }\n\n";

    QString callArgs;
    for (int i = 0; i < schema.params.size(); ++i) {
        const auto& p = schema.params[i];
        if (i > 0) callArgs += ", ";
        callArgs += "_" + p.name;
        
        if (p.type == "int") {
            out += "    let _" + p.name + " = parseInt(_nextToken());\n";
        } else if (p.type == "double") {
            out += "    let _" + p.name + " = parseFloat(_nextToken());\n";
        } else if (p.type == "string") {
            out += "    let _" + p.name + " = _nextToken();\n";
        } else if (p.type.startsWith("vector<")) {
            QString baseTypeStr = p.type.mid(7, p.type.length() - 8);
            QString castFunc = (baseTypeStr == "int") ? "parseInt" : ((baseTypeStr == "double") ? "parseFloat" : "");
            out += "    let _size_" + p.name + " = parseInt(_nextToken());\n";
            out += "    let _" + p.name + " = [];\n";
            out += "    for(let i=0; i<_size_" + p.name + "; ++i) {\n";
            if (castFunc.isEmpty()) {
                out += "        _" + p.name + ".push(_nextToken());\n";
            } else {
                out += "        _" + p.name + ".push(" + castFunc + "(_nextToken()));\n";
            }
            out += "    }\n";
        }
    }
    
    if (schema.returnType == "void") {
        out += "    " + schema.name + "(" + callArgs + ");\n";
    } else {
        out += "    let _result = " + schema.name + "(" + callArgs + ");\n";
        if (schema.returnType == "int" || schema.returnType == "double" || schema.returnType == "string") {
            out += "    print(_result);\n";
        } else if (schema.returnType.startsWith("vector<")) {
            out += "    print(_result.join(' '));\n";
        }
    }
    out += "})();\n";
    return out;
}

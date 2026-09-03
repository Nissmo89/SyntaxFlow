#pragma once
#include <QString>
#include <QStringList>
#include <QRegularExpression>
class OutputNormalizer {
public:
    static QStringList tokenize(const QString &s) {
        // Split by ANY whitespace
        return s.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    }

    static bool equals(const QString &actual,
                       const QString &expected) {
        return tokenize(actual) == tokenize(expected);
    }
    
    static QString normalizeError(const QString &errorOutput, const QString &languageId, int offset) {
        if (errorOutput.isEmpty()) return errorOutput;
        
        QString result = errorOutput;
        
        if (languageId == "python") {
            // Remove harness tracebacks before normalization
            QRegularExpression filterRe(QStringLiteral("[ \\t]*File\\s+\"[^\"]*user_code\\.py\".*?\\n.*?(?:\\n|$)"));
            result.replace(filterRe, QStringLiteral(""));

            // Normalize solution.py (user code) to user_code.py and apply offset
            QRegularExpression re(QStringLiteral("File\\s+\"[^\"]*(?:user_code|solution)\\.py\",\\s+line\\s+(\\d+)"));
            auto it = re.globalMatch(result);
            int diff = 0;
            while (it.hasNext()) {
                QRegularExpressionMatch match = it.next();
                int originalLine = match.captured(1).toInt();
                int newLine = originalLine - offset;
                if (newLine < 1) newLine = 1;
                
                QString replacement = "File \"user_code.py\", line " + QString::number(newLine);
                result.replace(match.capturedStart() + diff, match.capturedLength(), replacement);
                diff += replacement.length() - match.capturedLength();
            }
        } else if (languageId == "javascript" || languageId == "js") {
            // QuickJS error lines: (user_code.js:152) or SyntaxError: ... user_code.js:152
            QRegularExpression re(QStringLiteral("(user_code\\.js:)(\\d+)"));
            auto it = re.globalMatch(errorOutput);
            int diff = 0;
            while (it.hasNext()) {
                QRegularExpressionMatch match = it.next();
                int originalLine = match.captured(2).toInt();
                int newLine = originalLine - offset;
                if (newLine < 1) newLine = 1;
                
                QString replacement = match.captured(1) + QString::number(newLine);
                result.replace(match.capturedStart() + diff, match.capturedLength(), replacement);
                diff += replacement.length() - match.capturedLength();
            }
        } else if (languageId == "c" || languageId == "cpp" || languageId == "c++") {
            // Clang error lines: user_code.cpp:152:5: error: ...
            // Or user_code.c:152:
            QRegularExpression re(QStringLiteral("(user_code\\.(?:cpp|c|cc|cxx)):(\\d+)"));
            auto it = re.globalMatch(errorOutput);
            int diff = 0;
            while (it.hasNext()) {
                QRegularExpressionMatch match = it.next();
                int originalLine = match.captured(2).toInt();
                int newLine = originalLine - offset;
                if (newLine < 1) newLine = 1;
                
                QString replacement = match.captured(1) + ":" + QString::number(newLine);
                result.replace(match.capturedStart() + diff, match.capturedLength(), replacement);
                diff += replacement.length() - match.capturedLength();
            }
        }
        
        return result;
    }
};

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
        if (offset <= 0 || errorOutput.isEmpty()) return errorOutput;
        
        QString result = errorOutput;
        
        if (languageId == "python") {
            // Python traceback lines: File "user_code.py", line 152, in ...
            QRegularExpression re(QStringLiteral("(File\\s+\"[^\"]*user_code\\.py\",\\s+line\\s+)(\\d+)"));
            auto it = re.globalMatch(result);
            int diff = 0;
            while (it.hasNext()) {
                QRegularExpressionMatch match = it.next();
                int originalLine = match.captured(2).toInt();
                int newLine = originalLine - offset;
                if (newLine < 1) newLine = 1;
                
                QString replacement = match.captured(1) + QString::number(newLine);
                result.replace(match.capturedStart() + diff, match.capturedLength(), replacement);
                diff += replacement.length() - match.capturedLength();
                it = re.globalMatch(result, match.capturedStart() + diff);
            }
        } else if (languageId == "javascript" || languageId == "js") {
            // QuickJS error lines: (user_code.js:152) or SyntaxError: ... user_code.js:152
            QRegularExpression re(QStringLiteral("(user_code\\.js:)(\\d+)"));
            auto it = re.globalMatch(result);
            int diff = 0;
            while (it.hasNext()) {
                QRegularExpressionMatch match = it.next();
                int originalLine = match.captured(2).toInt();
                int newLine = originalLine - offset;
                if (newLine < 1) newLine = 1;
                
                QString replacement = match.captured(1) + QString::number(newLine);
                result.replace(match.capturedStart() + diff, match.capturedLength(), replacement);
                diff += replacement.length() - match.capturedLength();
                it = re.globalMatch(result, match.capturedStart() + diff);
            }
        } else if (languageId == "c" || languageId == "cpp" || languageId == "c++") {
            // Clang error lines: user_code.cpp:152:5: error: ...
            // Or user_code.c:152:
            QRegularExpression re(QStringLiteral("(user_code\\.(?:cpp|c|cc|cxx)):(\\d+)"));
            auto it = re.globalMatch(result);
            int diff = 0;
            while (it.hasNext()) {
                QRegularExpressionMatch match = it.next();
                int originalLine = match.captured(2).toInt();
                int newLine = originalLine - offset;
                if (newLine < 1) newLine = 1;
                
                QString replacement = match.captured(1) + ":" + QString::number(newLine);
                result.replace(match.capturedStart() + diff, match.capturedLength(), replacement);
                diff += replacement.length() - match.capturedLength();
                it = re.globalMatch(result, match.capturedStart() + diff);
            }
        }
        
        return result;
    }
};

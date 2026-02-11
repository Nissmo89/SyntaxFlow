#pragma once
#include <QString>
#include <QStringList>
#include <qregularexpression.h>
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
};

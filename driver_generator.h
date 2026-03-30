#ifndef DRIVER_GENERATOR_H
#define DRIVER_GENERATOR_H

#include <QString>
#include <QList>
#include <QJsonObject>
#include <QJsonArray>

// Represents a single parameter
struct ParamSchema {
    QString name;
    QString type; 
};

// Represents the problem method signature
struct MethodSchema {
    QString name;
    QString returnType;
    QList<ParamSchema> params;

    static MethodSchema fromJson(const QJsonObject& obj);
    bool isValid() const { return !name.isEmpty(); }
};

// Generates code to wrap functional solutions
class DriverGenerator {
public:
    // Generate the starter code template for the editor
    static QString generateTemplate(const MethodSchema& schema, const QString& languageId);

    // Generate the hidden driver code that calls the user's function
    static QString generateDriver(const MethodSchema& schema, const QString& languageId);

private:
    static QString generateCppDriver(const MethodSchema& schema);
    static QString generatePythonDriver(const MethodSchema& schema);
    static QString generateJsDriver(const MethodSchema& schema);
};

#endif // DRIVER_GENERATOR_H

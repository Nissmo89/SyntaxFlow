#pragma once

#include <QTcpServer>
#include <QTcpSocket>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMimeDatabase>
#include <QCoreApplication>
#include <QDebug>
#include <QUrl>

class HttpServer : public QTcpServer {
    Q_OBJECT
public:
    HttpServer(QObject *parent = nullptr) : QTcpServer(parent) {
        // Prefer the source directory for hot-reloading web code without rebuilding
        const QString appDir = QCoreApplication::applicationDirPath();
        m_docRoot = QDir(appDir + "/../web_editor").absolutePath();
        
        if (!QDir(m_docRoot).exists()) {
            m_docRoot = QDir(appDir).filePath("web_editor");
        }
        
        // Listen on any available port on localhost
        listen(QHostAddress::LocalHost, 0);
    }

    QString getBaseUrl() const {
        return QString("http://127.0.0.1:%1").arg(serverPort());
    }

    QString getUrlFor(const QString &file, const QString &queryParams = "") const {
        QString urlStr = getBaseUrl() + file;
        if (!queryParams.isEmpty()) {
            urlStr += "?" + queryParams;
        }
        return urlStr;
    }

protected:
    void incomingConnection(qintptr socketDescriptor) override {
        QTcpSocket *socket = new QTcpSocket(this);
        socket->setSocketDescriptor(socketDescriptor);
        
        connect(socket, &QTcpSocket::readyRead, this, [this, socket]() {
            if (socket->canReadLine()) {
                QString requestLine = socket->readLine();
                if (requestLine.startsWith("GET")) {
                    QString path = requestLine.split(' ').value(1, "/");
                    if (path.contains('?')) {
                        path = path.section('?', 0, 0);
                    }
                    
                    // Consume the rest of the HTTP headers
                    while (socket->canReadLine()) {
                        if (socket->readLine() == "\r\n") break;
                    }

                    // Sanitize path to avoid double slashes making QDir::filePath absolute
                    while(path.startsWith('/')) {
                        path = path.mid(1);
                    }
                    if (path.isEmpty()) {
                        path = "workspace.html";
                    }

                    QString fullPath = QDir(m_docRoot).filePath(path);
                    QFile file(fullPath);
                    if (file.exists() && file.open(QIODevice::ReadOnly)) {
                        QMimeDatabase db;
                        QString mime = db.mimeTypeForFile(fullPath).name();
                        QByteArray data = file.readAll();
                        
                        socket->write("HTTP/1.1 200 OK\r\n");
                        socket->write("Content-Type: " + mime.toUtf8() + "\r\n");
                        // Enable CORS in case WebEngine needs it
                        socket->write("Access-Control-Allow-Origin: *\r\n");
                        socket->write("Content-Length: " + QByteArray::number(data.size()) + "\r\n");
                        socket->write("Connection: close\r\n\r\n");
                        socket->write(data);
                    } else {
                        socket->write("HTTP/1.1 404 Not Found\r\nConnection: close\r\n\r\n");
                    }
                }
            }
            socket->disconnectFromHost();
        });
        connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    }

private:
    QString m_docRoot;
};

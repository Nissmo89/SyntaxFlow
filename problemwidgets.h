#ifndef PROBLEMWIDGETS_H
#define PROBLEMWIDGETS_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QPainter>
#include <QFrame>

struct ProblemData {
    QString id;
    QString title;
    QString difficulty;
    QString path;
    bool isSolved;
    bool isStarred;
    QStringList topics;
};

// A small widget for "Array", "DP" tags
class TagLabel : public QLabel {
public:
    TagLabel(const QString &text, QWidget *parent = nullptr);
};

class ProblemCard : public QFrame {
    Q_OBJECT
public:
    explicit ProblemCard(const ProblemData &data, QWidget *parent = nullptr);

signals:
    void openRequested(QString path);

protected:
    // We override paint to draw the "Difficulty Strip" on the left
    void paintEvent(QPaintEvent *event) override;

private:
    void setupUi(const ProblemData &data);
    QColor getDifficultyColor(const QString &diff);
    QColor difficultyColor;
};

class ProblemBrowser : public QWidget {
    Q_OBJECT
public:
    explicit ProblemBrowser(QWidget *parent = nullptr);
    void loadFromJson(const QString &filePath);

signals:
    void navigateToEditor(QString problemPath);

private:
    QVBoxLayout *listLayout;
    QWidget *scrollContent;
};

#endif // PROBLEMWIDGETS_H

#ifndef PROBLEMWIDGETS_H
#define PROBLEMWIDGETS_H

#include "progressmanager.h"
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
#include <QEvent>
#include <QMouseEvent>

struct ProblemData {
    QString id;
    QString title;
    QString difficulty;
    QString path;
    QStringList topics;

    // From user_progress.json
    bool isSolved  = false;
    bool isStarred = false;
    int  attempts  = 0;
};

// A small chip for "Array", "DP" tags — color-coded by category
class TagLabel : public QLabel {
public:
    TagLabel(const QString &text, QWidget *parent = nullptr);
};

class ProblemCard : public QFrame {
    Q_OBJECT
public:
    explicit ProblemCard(const ProblemData &data, QWidget *parent = nullptr);
    void updateSolvedState(bool solved);
    QLabel *statusLabel;

signals:
    void openRequested(QString path);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    bool event(QEvent *e) override;

private:
    void setupUi(const ProblemData &data);
    QString buildCardStyleSheet() const;
    QColor getDifficultyColor(const QString &diff);
    QColor difficultyColor;
    ProblemData m_data;
};

class ProblemBrowser : public QWidget {
    Q_OBJECT
public:
    explicit ProblemBrowser(ProgressManager *pm, QWidget *parent = nullptr);
    void loadFromJson(const QString &filePath);

signals:
    void navigateToEditor(QString problemPath);

private:
    void setupHeader();
    void updateHeaderStats();

    QVBoxLayout  *listLayout;
    QWidget      *scrollContent;
    QWidget      *m_headerWidget;
    QLabel       *m_statsLabel;
    ProgressManager *progressManager;
    QMap<QString, ProblemCard*> cardMap;

private slots:
    void onProgressChanged(const QString &problemId);
};

#endif // PROBLEMWIDGETS_H

#ifndef LAB_BROWSER_H
#define LAB_BROWSER_H

#include "lab_manager.h"
#include <QWidget>
#include <QListWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>

class LabBrowser : public QWidget
{
    Q_OBJECT

public:
    explicit LabBrowser(LabManager *manager, QWidget *parent = nullptr);

signals:
    void labSelected(const QString &labId);
    void createNewLab();

private slots:
    void onCreateClicked();
    void onSearchTextChanged(const QString &text);
    void onFilterChanged(int index);
    void onLabItemClicked(QListWidgetItem *item);
    void onLabItemDoubleClicked(QListWidgetItem *item);
    void onDeleteLab();
    void onStarLab();
    void onRefresh();

private:
    LabManager *m_manager;

    QLineEdit *m_searchBox;
    QComboBox *m_filterCombo;
    QPushButton *m_createButton;
    QListWidget *m_labList;

    QString m_currentFilter;
    QString m_selectedLabId;

    void setupUI();
    void refreshList();
    void populateList(const QList<CodeLab> &labs);
    QWidget *createLabCard(const CodeLab &lab);
};

#endif // LAB_BROWSER_H

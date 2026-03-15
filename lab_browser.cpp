#include "lab_browser.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QMessageBox>
#include <QInputDialog>
#include <QDateTime>

LabBrowser::LabBrowser(LabManager *manager, QWidget *parent)
    : QWidget(parent)
    , m_manager(manager)
{
    setupUI();
    refreshList();

    // Connect manager signals
    connect(m_manager, &LabManager::labCreated, this, &LabBrowser::onRefresh);
    connect(m_manager, &LabManager::labUpdated, this, &LabBrowser::onRefresh);
    connect(m_manager, &LabManager::labDeleted, this, &LabBrowser::onRefresh);
}

void LabBrowser::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    // Header
    auto *headerLayout = new QHBoxLayout;

    auto *titleLabel = new QLabel("Code Labs");
    titleLabel->setStyleSheet(R"(
        font-size: 24px;
        font-weight: bold;
        color: #e0e0e0;
    )");

    m_createButton = new QPushButton("+ New Lab");
    m_createButton->setStyleSheet(R"(
        QPushButton {
            background: #238636;
            color: white;
            border: none;
            border-radius: 6px;
            padding: 8px 16px;
            font-weight: 600;
        }
        QPushButton:hover {
            background: #2ea043;
        }
    )");
    m_createButton->setCursor(Qt::PointingHandCursor);

    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(m_createButton);

    // Search and Filter Bar
    auto *filterLayout = new QHBoxLayout;

    m_searchBox = new QLineEdit;
    m_searchBox->setPlaceholderText("Search labs...");
    m_searchBox->setStyleSheet(R"(
        QLineEdit {
            background: #1e1e1e;
            color: #d4d4d4;
            border: 1px solid #3a3a3a;
            border-radius: 5px;
            padding: 8px 12px;
            font-size: 13px;
        }
        QLineEdit:focus {
            border-color: #0078d4;
        }
    )");

    m_filterCombo = new QComboBox;
    m_filterCombo->addItem("All Languages", "all");
    m_filterCombo->addItem("⭐ Starred", "starred");
    m_filterCombo->addItem("C++", "cpp");
    m_filterCombo->addItem("Python", "python");
    m_filterCombo->addItem("Java", "java");
    m_filterCombo->addItem("JavaScript", "javascript");
    m_filterCombo->setStyleSheet(R"(
        QComboBox {
            background: #1e1e1e;
            color: #d4d4d4;
            border: 1px solid #3a3a3a;
            border-radius: 5px;
            padding: 8px 12px;
            min-width: 150px;
        }
        QComboBox::drop-down {
            border: none;
        }
        QComboBox QAbstractItemView {
            background: #1e1e1e;
            color: #d4d4d4;
            selection-background-color: #3a3a3a;
        }
    )");

    filterLayout->addWidget(m_searchBox, 1);
    filterLayout->addWidget(m_filterCombo);

    // Lab List
    m_labList = new QListWidget;
    m_labList->setStyleSheet(R"(
        QListWidget {
            background: transparent;
            border: none;
            outline: none;
        }
        QListWidget::item {
            background: #1e1e1e;
            border: 1px solid #2a2a2a;
            border-radius: 8px;
            margin: 5px 0;
            padding: 0;
        }
        QListWidget::item:hover {
            border-color: #404040;
            background: #252525;
        }
        QListWidget::item:selected {
            border-color: #0078d4;
            background: #1e1e1e;
        }
    )");
    m_labList->setSpacing(8);
    m_labList->setContextMenuPolicy(Qt::CustomContextMenu);

    // Assemble
    mainLayout->addLayout(headerLayout);
    mainLayout->addLayout(filterLayout);
    mainLayout->addWidget(m_labList);

    // Connections
    connect(m_createButton, &QPushButton::clicked, this, &LabBrowser::onCreateClicked);
    connect(m_searchBox, &QLineEdit::textChanged, this, &LabBrowser::onSearchTextChanged);
    connect(m_filterCombo, QOverload<int>::of(&QComboBox::activated), this, &LabBrowser::onFilterChanged);
    connect(m_labList, &QListWidget::itemClicked, this, &LabBrowser::onLabItemClicked);
    connect(m_labList, &QListWidget::itemDoubleClicked, this, &LabBrowser::onLabItemDoubleClicked);
    connect(m_labList, &QListWidget::customContextMenuRequested, this, [this](const QPoint &pos) {
        QListWidgetItem *item = m_labList->itemAt(pos);
        if (!item) return;

        m_selectedLabId = item->data(Qt::UserRole).toString();

        QMenu menu;
        menu.addAction("Open", this, [this]() { emit labSelected(m_selectedLabId); });
        menu.addAction("Rename", this, [this]() {
            bool ok;
            QString newTitle = QInputDialog::getText(this, "Rename Lab", "New name:",
                                                     QLineEdit::Normal, "", &ok);
            if (ok && !newTitle.isEmpty()) {
                m_manager->renameLab(m_selectedLabId, newTitle);
            }
        });
        menu.addSeparator();
        menu.addAction("⭐ Toggle Star", this, &LabBrowser::onStarLab);
        menu.addSeparator();
        menu.addAction("Delete", this, &LabBrowser::onDeleteLab);

        menu.exec(m_labList->mapToGlobal(pos));
    });
}

void LabBrowser::refreshList()
{
    QList<CodeLab> labs;

    QString filter = m_filterCombo->currentData().toString();
    QString search = m_searchBox->text();

    if (filter == "starred") {
        labs = m_manager->getStarredLabs();
    } else if (filter == "all") {
        labs = m_manager->getAllLabs();
    } else {
        labs = m_manager->getLabsByLanguage(filter);
    }

    if (!search.isEmpty()) {
        labs = m_manager->searchLabs(search);
    }

    populateList(labs);
}

void LabBrowser::populateList(const QList<CodeLab> &labs)
{
    m_labList->clear();

    for (const CodeLab &lab : labs) {
        auto *item = new QListWidgetItem(m_labList);
        item->setData(Qt::UserRole, lab.id);
        item->setSizeHint(QSize(0, 80));

        auto *card = createLabCard(lab);
        m_labList->setItemWidget(item, card);
    }

    if (labs.isEmpty()) {
        auto *item = new QListWidgetItem("No labs found");
        item->setForeground(QColor("#666"));
        item->setTextAlignment(Qt::AlignCenter);
        item->setFlags(Qt::NoItemFlags);
        m_labList->addItem(item);
    }
}

QWidget *LabBrowser::createLabCard(const CodeLab &lab)
{
    auto *card = new QWidget;
    auto *layout = new QVBoxLayout(card);
    layout->setContentsMargins(15, 10, 15, 10);
    layout->setSpacing(5);

    // Title row
    auto *titleRow = new QHBoxLayout;

    auto *titleLabel = new QLabel(lab.title);
    titleLabel->setStyleSheet("font-size: 14px; font-weight: 600; color: #e0e0e0;");

    auto *starLabel = new QLabel(lab.isStarred ? "⭐" : "");
    starLabel->setStyleSheet("font-size: 14px;");

    titleRow->addWidget(titleLabel);
    titleRow->addStretch();
    titleRow->addWidget(starLabel);

    // Info row
    auto *infoRow = new QHBoxLayout;

    auto *langLabel = new QLabel(lab.language);
    langLabel->setStyleSheet("color: #58a6ff; font-size: 12px;");

    auto *dateLabel = new QLabel(lab.modified.toString("MMM d, yyyy"));
    dateLabel->setStyleSheet("color: #888; font-size: 11px;");

    infoRow->addWidget(langLabel);
    infoRow->addStretch();
    infoRow->addWidget(dateLabel);

    layout->addLayout(titleRow);
    layout->addLayout(infoRow);

    return card;
}

void LabBrowser::onCreateClicked()
{
    emit createNewLab();
}

void LabBrowser::onSearchTextChanged(const QString &text)
{
    Q_UNUSED(text);
    refreshList();
}

void LabBrowser::onFilterChanged(int index)
{
    Q_UNUSED(index);
    refreshList();
}

void LabBrowser::onLabItemClicked(QListWidgetItem *item)
{
    m_selectedLabId = item->data(Qt::UserRole).toString();
}

void LabBrowser::onLabItemDoubleClicked(QListWidgetItem *item)
{
    QString id = item->data(Qt::UserRole).toString();
    emit labSelected(id);
}

void LabBrowser::onDeleteLab()
{
    auto reply = QMessageBox::question(this, "Delete Lab",
                                       "Are you sure you want to delete this lab?",
                                       QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        m_manager->deleteLab(m_selectedLabId);
    }
}

void LabBrowser::onStarLab()
{
    CodeLab lab = m_manager->loadLab(m_selectedLabId);
    lab.isStarred = !lab.isStarred;
    m_manager->saveLab(lab);
}

void LabBrowser::onRefresh()
{
    refreshList();
}

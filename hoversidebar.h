#ifndef HOVERSIDEBAR_H
#define HOVERSIDEBAR_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTimer>
#include <QVector>

//=============================================================================
// NavButton - Simple navigation button
//=============================================================================
class NavButton : public QWidget
{
    Q_OBJECT

public:
    explicit NavButton(const QString &iconText, const QString &label, QWidget *parent = nullptr);

    QSize sizeHint() const override;

    void setSelected(bool selected);
    bool isSelected() const { return m_selected; }

    void setBadge(int count);
    void setCollapsed(bool collapsed);
    void setAccentColor(const QColor &color) { m_accentColor = color; update(); }

signals:
    void clicked();

protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QString m_iconText;
    QString m_label;
    bool m_selected = false;
    bool m_hovered = false;
    bool m_pressed = false;
    bool m_collapsed = false;
    int m_badge = 0;
    QColor m_accentColor{100, 180, 255};
};

//=============================================================================
// ProfileWidget - User profile display
//=============================================================================
class ProfileWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProfileWidget(QWidget *parent = nullptr);

    QSize sizeHint() const override;

    void setCollapsed(bool collapsed);
    void setUserName(const QString &name);
    void setUserEmail(const QString &email);
    void setOnlineStatus(bool online);

signals:
    void clicked();

protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    QString m_userName = "User";
    QString m_userEmail = "user@email.com";
    bool m_online = true;
    bool m_collapsed = false;
    bool m_hovered = false;
};

//=============================================================================
// Divider - Simple horizontal line
//=============================================================================
class Divider : public QWidget
{
    Q_OBJECT

public:
    explicit Divider(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
};

//=============================================================================
// SectionLabel - Section header text
//=============================================================================
class SectionLabel : public QWidget
{
    Q_OBJECT

public:
    explicit SectionLabel(const QString &text, QWidget *parent = nullptr);

    QSize sizeHint() const override;
    void setCollapsed(bool collapsed);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QString m_text;
    bool m_collapsed = false;
};

//=============================================================================
// HoverSidebar - Main sidebar widget
//=============================================================================
class HoverSidebar : public QWidget
{
    Q_OBJECT

public:
    explicit HoverSidebar(QWidget *parent = nullptr);

    void addNavButton(NavButton *button);
    void setSelectedIndex(int index);
    int selectedIndex() const { return m_selectedIndex; }

    void setCollapsedWidth(int width);
    void setExpandedWidth(int width);

    bool isExpanded() const { return m_isExpanded; }

signals:
    void navigationChanged(int index);
    void expandedChanged(bool expanded);

protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    void updateChildStates(bool collapsed);
    void setExpanded(bool expanded);

    QVBoxLayout *m_mainLayout = nullptr;
    QVBoxLayout *m_navLayout = nullptr;
    QVector<NavButton*> m_navButtons;
    ProfileWidget *m_profile = nullptr;

    int m_selectedIndex = 0;
    int m_collapsedWidth = 60;
    int m_expandedWidth = 220;
    bool m_isExpanded = false;
};

#endif // HOVERSIDEBAR_H

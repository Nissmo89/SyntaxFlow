#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QList>

//=============================================================================
// NavButton
//=============================================================================
class NavButton : public QWidget
{
    Q_OBJECT
public:
    explicit NavButton(const QString &iconText, const QString &label, QWidget *parent = nullptr);

    QSize sizeHint() const override;
    void setSelected(bool selected);
    void setBadge(int count);
    void setCollapsed(bool collapsed);
    void setAccentColor(const QColor &color) { m_accentColor = color; update(); }



signals:
    void clicked();

protected:
    void paintEvent(QPaintEvent *) override;
    void enterEvent(QEnterEvent *) override;
    void leaveEvent(QEvent *) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QString m_iconText;
    QString m_label;
    QColor  m_accentColor = QColor(80, 140, 220);
    int     m_badge       = 0;
    bool    m_selected    = false;
    bool    m_hovered     = false;
    bool    m_pressed     = false;
    bool    m_collapsed   = true;
};

//=============================================================================
// ProfileWidget
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
    void paintEvent(QPaintEvent *) override;
    void enterEvent(QEnterEvent *) override;
    void leaveEvent(QEvent *) override;
    void mousePressEvent(QMouseEvent *) override;

private:
    QString m_userName  = "User";
    QString m_userEmail = "";
    bool    m_online    = true;
    bool    m_hovered   = false;
    bool    m_collapsed = true;
};

//=============================================================================
// Divider
//=============================================================================
class Divider : public QWidget
{
    Q_OBJECT
public:
    explicit Divider(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *) override;
};

//=============================================================================
// SectionLabel
//=============================================================================
class SectionLabel : public QWidget
{
    Q_OBJECT
public:
    explicit SectionLabel(const QString &text, QWidget *parent = nullptr);
    QSize sizeHint() const override;
    void setCollapsed(bool collapsed);
protected:
    void paintEvent(QPaintEvent *) override;
private:
    QString m_text;
    bool    m_collapsed = false;
};

//=============================================================================
// HoverSidebar
//=============================================================================
class HoverSidebar : public QWidget
{
    Q_OBJECT
public:
    explicit HoverSidebar(QWidget *parent = nullptr);

    // ── Main API ──────────────────────────────────────────────────────────
    // Add a primary nav item (top section). Returns its index for setSelectedIndex().
    int  addNavItem(const QString &iconText, const QString &label,
                   const QColor &accent = QColor(80, 140, 220));

    // Add a bottom utility item (settings, help, etc). Index is independent.
    int  addBottomItem(const QString &iconText, const QString &label,
                      const QColor &accent = QColor(160, 140, 200));

    // Badge on any nav item by index
    void setNavBadge(int index, int count);

    // ── Profile ───────────────────────────────────────────────────────────
    void setUserName(const QString &name);
    void setUserEmail(const QString &email);
    void setOnlineStatus(bool online);

    // ── Selection & sizing ───────────────────────────────────────────────
    void setSelectedIndex(int index);
    int  selectedIndex() const { return m_selectedIndex; }
    void setCollapsedWidth(int width);
    void setExpandedWidth(int width);
    void setExpanded(bool expanded);



signals:
    void navigationChanged(int index);
    void bottomItemClicked(int index);
    void expandedChanged(bool expanded);
    void profileClicked();

protected:
    void paintEvent(QPaintEvent *) override;
    void enterEvent(QEnterEvent *) override;
    void leaveEvent(QEvent *) override;

private:
    void updateChildStates(bool collapsed);

    int  m_collapsedWidth = 56;
    int  m_expandedWidth  = 220;
    int  m_selectedIndex  = -1;
    bool m_isExpanded     = false;

    QVBoxLayout   *m_mainLayout   = nullptr;
    QVBoxLayout   *m_navLayout    = nullptr;
    QVBoxLayout   *m_bottomLayout = nullptr;
    QLabel        *m_logoLabel    = nullptr;
    QLabel        *m_appNameLabel = nullptr;
    ProfileWidget *m_profile      = nullptr;

    QList<NavButton *> m_navButtons;    // top nav items
    QList<NavButton *> m_bottomButtons; // bottom utility items
    QList<SectionLabel *> m_sectionLabels;
};

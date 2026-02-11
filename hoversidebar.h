// hoversidebar.h
#ifndef HOVERSIDEBAR_H
#define HOVERSIDEBAR_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QList>
#include <QTimer>

// Forward declarations
class NavButton;
class QVBoxLayout;

//=============================================================================
// NavButton - Custom painted navigation button with modern styling
//=============================================================================
class NavButton : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal hoverProgress READ hoverProgress WRITE setHoverProgress)
    Q_PROPERTY(qreal pressProgress READ pressProgress WRITE setPressProgress)
    Q_PROPERTY(qreal glowIntensity READ glowIntensity WRITE setGlowIntensity)

public:
    explicit NavButton(const QString &iconText, const QString &label,
                       QWidget *parent = nullptr);

    void setSelected(bool selected);
    bool isSelected() const { return m_selected; }

    void setBadge(int count);
    void setCollapsed(bool collapsed);
    void setAccentColor(const QColor &color);

    qreal hoverProgress() const { return m_hoverProgress; }
    void setHoverProgress(qreal p);

    qreal pressProgress() const { return m_pressProgress; }
    void setPressProgress(qreal p);

    qreal glowIntensity() const { return m_glowIntensity; }
    void setGlowIntensity(qreal g);

    QSize sizeHint() const override;

    // NavButton *btnExplorer = nullptr;


signals:
    void clicked();

protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void animateHover(bool in);
    void animatePress(bool pressed);
    void startGlowPulse();
    void stopGlowPulse();

    QString m_iconText;
    QString m_label;
    bool m_selected = false;
    bool m_collapsed = true;
    int m_badge = 0;
    QColor m_accentColor{100, 180, 255};  // Default blue accent

    qreal m_hoverProgress = 0.0;
    qreal m_pressProgress = 0.0;
    qreal m_glowIntensity = 0.0;

    QPropertyAnimation *m_hoverAnim;
    QPropertyAnimation *m_pressAnim;
    QPropertyAnimation *m_glowAnim;
    QTimer *m_glowTimer;
};

//=============================================================================
// ProfileWidget - User avatar and info display
//=============================================================================
class ProfileWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal hoverProgress READ hoverProgress WRITE setHoverProgress)

public:
    explicit ProfileWidget(QWidget *parent = nullptr);

    void setCollapsed(bool collapsed);
    void setUserName(const QString &name);
    void setUserEmail(const QString &email);
    void setOnlineStatus(bool online);

    qreal hoverProgress() const { return m_hoverProgress; }
    void setHoverProgress(qreal p);

    QSize sizeHint() const override;

signals:
    void clicked();

protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    QString m_userName = "John Doe";
    QString m_userEmail = "john@example.com";
    bool m_collapsed = true;
    bool m_online = true;
    qreal m_hoverProgress = 0.0;
    QPropertyAnimation *m_hoverAnim;
};

//=============================================================================
// Divider - Styled separator line
//=============================================================================
class Divider : public QWidget
{
    Q_OBJECT

public:
    explicit Divider(QWidget *parent = nullptr);
    QSize sizeHint() const override { return QSize(-1, 1); }

protected:
    void paintEvent(QPaintEvent *event) override;
};

//=============================================================================
// SectionLabel - Collapsible section header
//=============================================================================
class SectionLabel : public QWidget
{
    Q_OBJECT

public:
    explicit SectionLabel(const QString &text, QWidget *parent = nullptr);

    void setCollapsed(bool collapsed);
    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QString m_text;
    bool m_collapsed = true;
};

//=============================================================================
// HoverSidebar - Main sidebar container
//=============================================================================
class HoverSidebar : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal shadowOpacity READ shadowOpacity WRITE setShadowOpacity)

public:
    explicit HoverSidebar(QWidget *parent = nullptr);

    void setCollapsedWidth(int width);
    void setExpandedWidth(int width);

    void addNavButton(NavButton *button);
    void setSelectedIndex(int index);

    qreal shadowOpacity() const { return m_shadowOpacity; }
    void setShadowOpacity(qreal opacity);

signals:
    void navigationChanged(int index);

protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    void animateTo(int targetWidth);
    void updateChildStates(bool collapsed);

    QVBoxLayout *m_mainLayout;
    QVBoxLayout *m_navLayout;
    ProfileWidget *m_profile;

    QPropertyAnimation *m_animMin;
    QPropertyAnimation *m_animMax;
    QPropertyAnimation *m_shadowAnim;
    QParallelAnimationGroup *m_animGroup;

    QList<NavButton*> m_navButtons;
    int m_selectedIndex = 0;

    int m_collapsedWidth = 64;
    int m_expandedWidth = 240;
    bool m_isExpanded = false;
    qreal m_shadowOpacity = 0.0;
};

#endif // HOVERSIDEBAR_H

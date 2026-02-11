// hoversidebar.cpp
#include "hoversidebar.h"
#include <QVBoxLayout>
#include <QPainter>
#include <QPainterPath>
#include <QLinearGradient>
#include <QRadialGradient>
#include <QMouseEvent>
#include <QtMath>

//=============================================================================
// NavButton Implementation
//=============================================================================
NavButton::NavButton(const QString &iconText, const QString &label, QWidget *parent)
    : QWidget(parent)
    , m_iconText(iconText)
    , m_label(label)
{
    setMouseTracking(true);
    setCursor(Qt::PointingHandCursor);
    setFixedHeight(44);

    // Hover animation
    m_hoverAnim = new QPropertyAnimation(this, "hoverProgress", this);
    m_hoverAnim->setDuration(150);
    m_hoverAnim->setEasingCurve(QEasingCurve::OutCubic);

    // Press animation
    m_pressAnim = new QPropertyAnimation(this, "pressProgress", this);
    m_pressAnim->setDuration(100);
    m_pressAnim->setEasingCurve(QEasingCurve::OutCubic);

    // Glow animation for selected state
    m_glowAnim = new QPropertyAnimation(this, "glowIntensity", this);
    m_glowAnim->setDuration(1500);
    m_glowAnim->setEasingCurve(QEasingCurve::InOutSine);

    m_glowTimer = new QTimer(this);
    connect(m_glowTimer, &QTimer::timeout, this, &NavButton::startGlowPulse);
}

QSize NavButton::sizeHint() const
{
    return QSize(200, 44);
}

void NavButton::setSelected(bool selected)
{
    if (m_selected != selected) {
        m_selected = selected;
        if (m_selected) {
            startGlowPulse();
        } else {
            stopGlowPulse();
        }
        update();
    }
}

void NavButton::setBadge(int count)
{
    m_badge = count;
    update();
}

void NavButton::setCollapsed(bool collapsed)
{
    m_collapsed = collapsed;
    update();
}

void NavButton::setAccentColor(const QColor &color)
{
    m_accentColor = color;
    update();
}

void NavButton::setHoverProgress(qreal p)
{
    m_hoverProgress = p;
    update();
}

void NavButton::setPressProgress(qreal p)
{
    m_pressProgress = p;
    update();
}

void NavButton::setGlowIntensity(qreal g)
{
    m_glowIntensity = g;
    update();
}

void NavButton::animateHover(bool in)
{
    m_hoverAnim->stop();
    m_hoverAnim->setStartValue(m_hoverProgress);
    m_hoverAnim->setEndValue(in ? 1.0 : 0.0);
    m_hoverAnim->start();
}

void NavButton::animatePress(bool pressed)
{
    m_pressAnim->stop();
    m_pressAnim->setStartValue(m_pressProgress);
    m_pressAnim->setEndValue(pressed ? 1.0 : 0.0);
    m_pressAnim->start();
}

void NavButton::startGlowPulse()
{
    m_glowAnim->stop();
    m_glowAnim->setStartValue(0.3);
    m_glowAnim->setEndValue(0.8);
    m_glowAnim->setLoopCount(-1);  // Infinite loop
    m_glowAnim->start();
}

void NavButton::stopGlowPulse()
{
    m_glowAnim->stop();
    m_glowTimer->stop();
    m_glowIntensity = 0.0;
    update();
}

void NavButton::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setRenderHint(QPainter::TextAntialiasing);

    QRectF r = rect().adjusted(4, 2, -4, -2);
    qreal radius = 10;

    // Calculate colors based on state
    QColor bgColor(255, 255, 255, 0);
    QColor borderColor(255, 255, 255, 0);

    if (m_selected) {
        // Selected state - gradient background
        QLinearGradient grad(r.topLeft(), r.bottomRight());
        grad.setColorAt(0, QColor(m_accentColor.red(), m_accentColor.green(),
                                  m_accentColor.blue(), 40 + int(m_glowIntensity * 30)));
        grad.setColorAt(1, QColor(m_accentColor.red(), m_accentColor.green(),
                                  m_accentColor.blue(), 20 + int(m_glowIntensity * 15)));

        // Glow effect
        if (m_glowIntensity > 0) {
            QPainterPath glowPath;
            glowPath.addRoundedRect(r.adjusted(-2, -2, 2, 2), radius + 2, radius + 2);
            p.setPen(Qt::NoPen);
            p.setBrush(QColor(m_accentColor.red(), m_accentColor.green(),
                              m_accentColor.blue(), int(m_glowIntensity * 40)));
            p.drawPath(glowPath);
        }

        p.setBrush(grad);
        borderColor = QColor(m_accentColor.red(), m_accentColor.green(),
                             m_accentColor.blue(), 100);
    } else {
        // Hover state
        int alpha = int(m_hoverProgress * 25);
        bgColor = QColor(255, 255, 255, alpha);
        p.setBrush(bgColor);
    }

    // Press effect - slight shrink
    if (m_pressProgress > 0) {
        qreal shrink = m_pressProgress * 2;
        r.adjust(shrink, shrink, -shrink, -shrink);
    }

    // Draw background
    QPainterPath path;
    path.addRoundedRect(r, radius, radius);
    p.setPen(m_selected ? QPen(borderColor, 1) : Qt::NoPen);
    p.drawPath(path);

    // Left accent bar for selected
    if (m_selected) {
        QRectF accentBar(r.left(), r.top() + 8, 3, r.height() - 16);
        QPainterPath barPath;
        barPath.addRoundedRect(accentBar, 1.5, 1.5);
        p.fillPath(barPath, m_accentColor);
    }

    // Icon
    QFont iconFont = font();
    iconFont.setPixelSize(18);
    iconFont.setWeight(QFont::Normal);
    p.setFont(iconFont);

    QColor iconColor = m_selected ? m_accentColor :
                           QColor(180 + int(m_hoverProgress * 40),
                                  180 + int(m_hoverProgress * 40),
                                  180 + int(m_hoverProgress * 40));
    p.setPen(iconColor);

    QRectF iconRect(r.left() + 12, r.top(), 24, r.height());
    p.drawText(iconRect, Qt::AlignCenter, m_iconText);

    // Label (when expanded)
    if (!m_collapsed) {
        QFont labelFont = font();
        labelFont.setPixelSize(13);
        labelFont.setWeight(m_selected ? QFont::DemiBold : QFont::Normal);
        p.setFont(labelFont);

        QColor textColor = m_selected ? QColor(255, 255, 255) :
                               QColor(200 + int(m_hoverProgress * 30),
                                      200 + int(m_hoverProgress * 30),
                                      200 + int(m_hoverProgress * 30));
        p.setPen(textColor);

        QRectF textRect(r.left() + 48, r.top(), r.width() - 60, r.height());
        p.drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, m_label);
    }

    // Badge
    if (m_badge > 0) {
        QFont badgeFont = font();
        badgeFont.setPixelSize(10);
        badgeFont.setWeight(QFont::Bold);
        p.setFont(badgeFont);

        QString badgeText = m_badge > 99 ? "99+" : QString::number(m_badge);
        QFontMetrics fm(badgeFont);
        int textWidth = fm.horizontalAdvance(badgeText);
        int badgeWidth = qMax(18, textWidth + 8);

        QRectF badgeRect;
        if (m_collapsed) {
            badgeRect = QRectF(r.right() - 14, r.top() + 6, badgeWidth, 16);
        } else {
            badgeRect = QRectF(r.right() - badgeWidth - 8,
                               (r.height() - 16) / 2 + r.top(), badgeWidth, 16);
        }

        // Badge background with gradient
        QLinearGradient badgeGrad(badgeRect.topLeft(), badgeRect.bottomRight());
        badgeGrad.setColorAt(0, QColor(255, 90, 90));
        badgeGrad.setColorAt(1, QColor(220, 60, 60));

        QPainterPath badgePath;
        badgePath.addRoundedRect(badgeRect, 8, 8);
        p.fillPath(badgePath, badgeGrad);

        p.setPen(Qt::white);
        p.drawText(badgeRect, Qt::AlignCenter, badgeText);
    }
}

void NavButton::enterEvent(QEnterEvent *)
{
    animateHover(true);
}

void NavButton::leaveEvent(QEvent *)
{
    animateHover(false);
    animatePress(false);
}

void NavButton::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        animatePress(true);
    }
}

void NavButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        animatePress(false);
        if (rect().contains(event->pos())) {
            emit clicked();
        }
    }
}

//=============================================================================
// ProfileWidget Implementation
//=============================================================================
ProfileWidget::ProfileWidget(QWidget *parent)
    : QWidget(parent)
{
    setMouseTracking(true);
    setCursor(Qt::PointingHandCursor);
    setFixedHeight(56);

    m_hoverAnim = new QPropertyAnimation(this, "hoverProgress", this);
    m_hoverAnim->setDuration(150);
    m_hoverAnim->setEasingCurve(QEasingCurve::OutCubic);
}

QSize ProfileWidget::sizeHint() const
{
    return QSize(200, 56);
}

void ProfileWidget::setCollapsed(bool collapsed)
{
    m_collapsed = collapsed;
    update();
}

void ProfileWidget::setUserName(const QString &name)
{
    m_userName = name;
    update();
}

void ProfileWidget::setUserEmail(const QString &email)
{
    m_userEmail = email;
    update();
}

void ProfileWidget::setOnlineStatus(bool online)
{
    m_online = online;
    update();
}

void ProfileWidget::setHoverProgress(qreal p)
{
    m_hoverProgress = p;
    update();
}

void ProfileWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setRenderHint(QPainter::TextAntialiasing);

    QRectF r = rect().adjusted(4, 4, -4, -4);

    // Hover background
    if (m_hoverProgress > 0) {
        QColor bgColor(255, 255, 255, int(m_hoverProgress * 15));
        QPainterPath path;
        path.addRoundedRect(r, 10, 10);
        p.fillPath(path, bgColor);
    }

    // Avatar
    qreal avatarSize = 36;
    QRectF avatarRect(r.left() + 8, (r.height() - avatarSize) / 2 + r.top(),
                      avatarSize, avatarSize);

    // Avatar gradient background
    QLinearGradient avatarGrad(avatarRect.topLeft(), avatarRect.bottomRight());
    avatarGrad.setColorAt(0, QColor(100, 180, 255));
    avatarGrad.setColorAt(1, QColor(80, 140, 220));

    QPainterPath avatarPath;
    avatarPath.addEllipse(avatarRect);
    p.fillPath(avatarPath, avatarGrad);

    // Avatar initials
    QFont initialsFont = font();
    initialsFont.setPixelSize(14);
    initialsFont.setWeight(QFont::DemiBold);
    p.setFont(initialsFont);
    p.setPen(Qt::white);

    QString initials;
    QStringList parts = m_userName.split(' ');
    for (const QString &part : parts) {
        if (!part.isEmpty()) {
            initials += part[0].toUpper();
            if (initials.length() >= 2) break;
        }
    }
    p.drawText(avatarRect, Qt::AlignCenter, initials);

    // Online indicator
    qreal indicatorSize = 10;
    QRectF indicatorRect(avatarRect.right() - indicatorSize + 2,
                         avatarRect.bottom() - indicatorSize + 2,
                         indicatorSize, indicatorSize);

    QPainterPath indicatorPath;
    indicatorPath.addEllipse(indicatorRect);

    // White border
    p.setPen(QPen(QColor(30, 30, 30), 2));
    p.setBrush(m_online ? QColor(80, 200, 120) : QColor(150, 150, 150));
    p.drawEllipse(indicatorRect);

    // User info (when expanded)
    if (!m_collapsed) {
        QFont nameFont = font();
        nameFont.setPixelSize(12);
        nameFont.setWeight(QFont::DemiBold);
        p.setFont(nameFont);
        p.setPen(QColor(230, 230, 230));

        QRectF nameRect(avatarRect.right() + 10, r.top() + 8,
                        r.width() - avatarRect.width() - 24, 16);
        p.drawText(nameRect, Qt::AlignVCenter | Qt::AlignLeft,
                   p.fontMetrics().elidedText(m_userName, Qt::ElideRight,
                                              int(nameRect.width())));

        QFont emailFont = font();
        emailFont.setPixelSize(10);
        p.setFont(emailFont);
        p.setPen(QColor(140, 140, 140));

        QRectF emailRect(avatarRect.right() + 10, nameRect.bottom() + 2,
                         r.width() - avatarRect.width() - 24, 14);
        p.drawText(emailRect, Qt::AlignVCenter | Qt::AlignLeft,
                   p.fontMetrics().elidedText(m_userEmail, Qt::ElideRight,
                                              int(emailRect.width())));
    }
}

void ProfileWidget::enterEvent(QEnterEvent *)
{
    m_hoverAnim->stop();
    m_hoverAnim->setStartValue(m_hoverProgress);
    m_hoverAnim->setEndValue(1.0);
    m_hoverAnim->start();
}

void ProfileWidget::leaveEvent(QEvent *)
{
    m_hoverAnim->stop();
    m_hoverAnim->setStartValue(m_hoverProgress);
    m_hoverAnim->setEndValue(0.0);
    m_hoverAnim->start();
}

void ProfileWidget::mousePressEvent(QMouseEvent *)
{
    emit clicked();
}

//=============================================================================
// Divider Implementation
//=============================================================================
Divider::Divider(QWidget *parent)
    : QWidget(parent)
{
    setFixedHeight(1);
}

void Divider::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    // Gradient fade on edges
    QLinearGradient grad(0, 0, width(), 0);
    grad.setColorAt(0, QColor(80, 80, 80, 0));
    grad.setColorAt(0.1, QColor(80, 80, 80, 255));
    grad.setColorAt(0.9, QColor(80, 80, 80, 255));
    grad.setColorAt(1, QColor(80, 80, 80, 0));

    p.fillRect(rect(), grad);
}

//=============================================================================
// SectionLabel Implementation
//=============================================================================
SectionLabel::SectionLabel(const QString &text, QWidget *parent)
    : QWidget(parent)
    , m_text(text)
{
    setFixedHeight(28);
}

QSize SectionLabel::sizeHint() const
{
    return QSize(200, 28);
}

void SectionLabel::setCollapsed(bool collapsed)
{
    m_collapsed = collapsed;
    update();
}

void SectionLabel::paintEvent(QPaintEvent *)
{
    if (m_collapsed) return;  // Hide when collapsed

    QPainter p(this);
    p.setRenderHint(QPainter::TextAntialiasing);

    QFont labelFont = font();
    labelFont.setPixelSize(10);
    labelFont.setWeight(QFont::DemiBold);
    labelFont.setLetterSpacing(QFont::AbsoluteSpacing, 1);
    p.setFont(labelFont);

    p.setPen(QColor(120, 120, 120));
    p.drawText(rect().adjusted(20, 0, 0, 0),
               Qt::AlignVCenter | Qt::AlignLeft, m_text.toUpper());
}

//=============================================================================
// HoverSidebar Implementation
//=============================================================================
HoverSidebar::HoverSidebar(QWidget *parent)
    : QWidget(parent)
{
    setMouseTracking(true);
    setAttribute(Qt::WA_Hover);
    setMinimumWidth(m_collapsedWidth);
    setMaximumWidth(m_collapsedWidth);

    // Animation setup
    m_animGroup = new QParallelAnimationGroup(this);

    m_animMin = new QPropertyAnimation(this, "minimumWidth", this);
    m_animMax = new QPropertyAnimation(this, "maximumWidth", this);
    m_shadowAnim = new QPropertyAnimation(this, "shadowOpacity", this);

    for (auto *anim : {m_animMin, m_animMax, m_shadowAnim}) {
        anim->setDuration(250);
        anim->setEasingCurve(QEasingCurve::OutCubic);
    }

    m_animGroup->addAnimation(m_animMin);
    m_animGroup->addAnimation(m_animMax);
    m_animGroup->addAnimation(m_shadowAnim);

    // Layout
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setSpacing(0);
    m_mainLayout->setContentsMargins(0, 8, 0, 8);

    // Logo/Brand area
    auto *logoWidget = new QWidget(this);
    logoWidget->setFixedHeight(48);
    m_mainLayout->addWidget(logoWidget);

    // Section label
    auto *mainSection = new SectionLabel("Main Menu", this);
    m_mainLayout->addWidget(mainSection);

    // Navigation buttons container
    auto *navContainer = new QWidget(this);
    m_navLayout = new QVBoxLayout(navContainer);
    m_navLayout->setSpacing(2);
    m_navLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->addWidget(navContainer);

    // Create navigation buttons
    auto *btnExplorer = new NavButton("📁", "Explorer", this);
    auto *btnSearch = new NavButton("🔍", "Search", this);
    auto *btnGit = new NavButton("⎇", "Source Control", this);
    btnGit->setBadge(3);
    auto *btnDebug = new NavButton("🐛", "Debug", this);
    auto *btnExtensions = new NavButton("🧩", "Extensions", this);
    btnExtensions->setBadge(12);

    addNavButton(btnExplorer);
    addNavButton(btnSearch);
    addNavButton(btnGit);
    addNavButton(btnDebug);
    addNavButton(btnExtensions);

    // Set first button as selected
    setSelectedIndex(0);

    // Spacer
    m_mainLayout->addStretch();

    // Bottom section
    auto *bottomSection = new SectionLabel("Account", this);
    m_mainLayout->addWidget(bottomSection);

    // Settings button
    auto *btnSettings = new NavButton("⚙️", "Settings", this);
    btnSettings->setAccentColor(QColor(180, 140, 255));
    m_navLayout->addWidget(btnSettings);

    // Divider
    m_mainLayout->addWidget(new Divider(this));

    // Profile widget
    m_profile = new ProfileWidget(this);
    m_profile->setUserName("John Developer");
    m_profile->setUserEmail("john@company.dev");
    m_mainLayout->addWidget(m_profile);

    // Store section labels for collapse/expand
    m_mainLayout->itemAt(1)->widget()->setProperty("isSection", true);
}

void HoverSidebar::addNavButton(NavButton *button)
{
    m_navButtons.append(button);
    m_navLayout->addWidget(button);

    int index = m_navButtons.size() - 1;
    connect(button, &NavButton::clicked, this, [this, index]() {
        setSelectedIndex(index);
        emit navigationChanged(index);
    });
}

void HoverSidebar::setSelectedIndex(int index)
{
    if (index >= 0 && index < m_navButtons.size()) {
        for (int i = 0; i < m_navButtons.size(); ++i) {
            m_navButtons[i]->setSelected(i == index);
        }
        m_selectedIndex = index;
    }
}

void HoverSidebar::setCollapsedWidth(int width)
{
    m_collapsedWidth = width;
    if (!m_isExpanded) {
        setFixedWidth(m_collapsedWidth);
    }
}

void HoverSidebar::setExpandedWidth(int width)
{
    m_expandedWidth = width;
    if (m_isExpanded) {
        setFixedWidth(m_expandedWidth);
    }
}

void HoverSidebar::setShadowOpacity(qreal opacity)
{
    m_shadowOpacity = opacity;
    update();
}

void HoverSidebar::animateTo(int targetWidth)
{
    m_animGroup->stop();

    m_animMin->setStartValue(minimumWidth());
    m_animMin->setEndValue(targetWidth);

    m_animMax->setStartValue(maximumWidth());
    m_animMax->setEndValue(targetWidth);

    m_shadowAnim->setStartValue(m_shadowOpacity);
    m_shadowAnim->setEndValue(targetWidth == m_expandedWidth ? 0.4 : 0.0);

    m_animGroup->start();
}

void HoverSidebar::updateChildStates(bool collapsed)
{
    for (NavButton *btn : m_navButtons) {
        btn->setCollapsed(collapsed);
    }
    m_profile->setCollapsed(collapsed);

    // Update all section labels
    for (int i = 0; i < m_mainLayout->count(); ++i) {
        auto *item = m_mainLayout->itemAt(i);
        if (item && item->widget()) {
            if (auto *section = qobject_cast<SectionLabel*>(item->widget())) {
                section->setCollapsed(collapsed);
            }
        }
    }
}

void HoverSidebar::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // Background gradient
    QLinearGradient bgGrad(0, 0, 0, height());
    bgGrad.setColorAt(0, QColor(28, 28, 32));
    bgGrad.setColorAt(1, QColor(22, 22, 26));
    p.fillRect(rect(), bgGrad);

    // Right border with gradient
    QLinearGradient borderGrad(0, 0, 0, height());
    borderGrad.setColorAt(0, QColor(60, 60, 65));
    borderGrad.setColorAt(0.5, QColor(50, 50, 55));
    borderGrad.setColorAt(1, QColor(40, 40, 45));

    p.setPen(QPen(QBrush(borderGrad), 1));
    p.drawLine(width() - 1, 0, width() - 1, height());

    // Drop shadow when expanded
    if (m_shadowOpacity > 0) {
        QLinearGradient shadowGrad(width(), 0, width() + 20, 0);
        shadowGrad.setColorAt(0, QColor(0, 0, 0, int(80 * m_shadowOpacity)));
        shadowGrad.setColorAt(1, QColor(0, 0, 0, 0));

        QPainter shadowPainter(this);
        // Note: Shadow would need to be drawn on parent or use graphics effects
    }
}

void HoverSidebar::enterEvent(QEnterEvent *)
{
    if (!m_isExpanded) {
        m_isExpanded = true;
        raise();
        updateChildStates(false);
        animateTo(m_expandedWidth);
    }
}

void HoverSidebar::leaveEvent(QEvent *)
{
    if (m_isExpanded) {
        m_isExpanded = false;
        updateChildStates(true);
        animateTo(m_collapsedWidth);
    }
}

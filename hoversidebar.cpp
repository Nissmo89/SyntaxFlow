#include "hoversidebar.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>

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
}

QSize NavButton::sizeHint() const { return QSize(200, 44); }

void NavButton::setSelected(bool selected)
{
    if (m_selected != selected) { m_selected = selected; update(); }
}

void NavButton::setBadge(int count) { m_badge = count; update(); }

void NavButton::setCollapsed(bool collapsed) { m_collapsed = collapsed; update(); }

void NavButton::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setRenderHint(QPainter::TextAntialiasing);

    QRectF r = rect().adjusted(6, 2, -6, -2);
    const qreal radius = 8;

    // Background
    QColor bgColor;
    if (m_selected)      bgColor = QColor(m_accentColor.red(), m_accentColor.green(), m_accentColor.blue(), 40);
    else if (m_pressed)  bgColor = QColor(255, 255, 255, 20);
    else if (m_hovered)  bgColor = QColor(255, 255, 255, 12);
    else                 bgColor = Qt::transparent;

    if (bgColor != Qt::transparent) {
        QPainterPath path;
        path.addRoundedRect(r, radius, radius);
        p.fillPath(path, bgColor);
    }

    // Left accent bar for selected
    if (m_selected) {
        QRectF accentBar(r.left() + 2, r.top() + 10, 3, r.height() - 20);
        QPainterPath barPath;
        barPath.addRoundedRect(accentBar, 1.5, 1.5);
        p.fillPath(barPath, m_accentColor);
    }

    // Icon
    QFont iconFont = font();
    iconFont.setPixelSize(18);
    p.setFont(iconFont);
    QColor iconColor = m_selected ? m_accentColor
                                  : (m_hovered ? QColor(220, 220, 220) : QColor(160, 160, 160));
    p.setPen(iconColor);
    p.drawText(QRectF(r.left() + 12, r.top(), 24, r.height()), Qt::AlignCenter, m_iconText);

    // Label (expanded only)
    if (!m_collapsed) {
        QFont labelFont = font();
        labelFont.setPixelSize(13);
        labelFont.setWeight(m_selected ? QFont::DemiBold : QFont::Normal);
        p.setFont(labelFont);
        QColor textColor = m_selected ? QColor(255, 255, 255)
                                      : (m_hovered ? QColor(230, 230, 230) : QColor(180, 180, 180));
        p.setPen(textColor);
        p.drawText(QRectF(r.left() + 48, r.top(), r.width() - 60, r.height()),
                   Qt::AlignVCenter | Qt::AlignLeft, m_label);
    }

    // Badge
    if (m_badge > 0) {
        QFont badgeFont = font();
        badgeFont.setPixelSize(10);
        badgeFont.setWeight(QFont::Bold);
        p.setFont(badgeFont);

        QString badgeText = m_badge > 99 ? "99+" : QString::number(m_badge);
        int badgeWidth = qMax(18, QFontMetrics(badgeFont).horizontalAdvance(badgeText) + 8);

        QRectF badgeRect = m_collapsed
                               ? QRectF(r.right() - 12, r.top() + 6, badgeWidth, 16)
                               : QRectF(r.right() - badgeWidth - 8, (r.height() - 16) / 2 + r.top(), badgeWidth, 16);

        QPainterPath badgePath;
        badgePath.addRoundedRect(badgeRect, 8, 8);
        p.fillPath(badgePath, QColor(220, 70, 70));
        p.setPen(Qt::white);
        p.drawText(badgeRect, Qt::AlignCenter, badgeText);
    }
}

void NavButton::enterEvent(QEnterEvent *)  { m_hovered = true;  update(); }
void NavButton::leaveEvent(QEvent *)       { m_hovered = false; m_pressed = false; update(); }

void NavButton::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) { m_pressed = true; update(); }
}

void NavButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_pressed = false;
        update();
        if (rect().contains(event->pos())) emit clicked();
    }
}

//=============================================================================
// ProfileWidget Implementation
//=============================================================================
ProfileWidget::ProfileWidget(QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true);
    setCursor(Qt::PointingHandCursor);
    setFixedHeight(56);
}

QSize ProfileWidget::sizeHint() const { return QSize(200, 56); }

void ProfileWidget::setCollapsed(bool collapsed) { m_collapsed = collapsed; update(); }
void ProfileWidget::setUserName(const QString &name)   { m_userName = name; update(); }
void ProfileWidget::setUserEmail(const QString &email) { m_userEmail = email; update(); }
void ProfileWidget::setOnlineStatus(bool online)       { m_online = online; update(); }

void ProfileWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setRenderHint(QPainter::TextAntialiasing);

    QRectF r = rect().adjusted(6, 4, -6, -4);

    if (m_hovered) {
        QPainterPath path;
        path.addRoundedRect(r, 8, 8);
        p.fillPath(path, QColor(255, 255, 255, 10));
    }

    const qreal avatarSize = 36;
    QRectF avatarRect(r.left() + 8, (r.height() - avatarSize) / 2 + r.top(), avatarSize, avatarSize);

    QPainterPath avatarPath;
    avatarPath.addEllipse(avatarRect);
    p.fillPath(avatarPath, QColor(80, 140, 200));

    QFont initialsFont = font();
    initialsFont.setPixelSize(14);
    initialsFont.setWeight(QFont::DemiBold);
    p.setFont(initialsFont);
    p.setPen(Qt::white);

    QString initials;
    for (const QString &part : m_userName.split(' ')) {
        if (!part.isEmpty()) { initials += part[0].toUpper(); if (initials.length() >= 2) break; }
    }
    if (initials.isEmpty() && !m_userName.isEmpty()) initials = m_userName[0].toUpper();
    p.drawText(avatarRect, Qt::AlignCenter, initials);

    // Online dot
    const qreal dotSize = 10;
    QRectF dotRect(avatarRect.right() - dotSize + 2, avatarRect.bottom() - dotSize + 2, dotSize, dotSize);
    p.setPen(QPen(QColor(30, 30, 35), 2));
    p.setBrush(m_online ? QColor(80, 180, 100) : QColor(130, 130, 130));
    p.drawEllipse(dotRect);

    if (!m_collapsed) {
        QFont nameFont = font();
        nameFont.setPixelSize(12);
        nameFont.setWeight(QFont::DemiBold);
        p.setFont(nameFont);
        p.setPen(QColor(220, 220, 220));
        QRectF nameRect(avatarRect.right() + 10, r.top() + 10, r.width() - avatarRect.width() - 30, 16);
        p.drawText(nameRect, Qt::AlignVCenter | Qt::AlignLeft,
                   p.fontMetrics().elidedText(m_userName, Qt::ElideRight, int(nameRect.width())));

        QFont emailFont = font();
        emailFont.setPixelSize(10);
        p.setFont(emailFont);
        p.setPen(QColor(130, 130, 130));
        QRectF emailRect(avatarRect.right() + 10, nameRect.bottom() + 2, r.width() - avatarRect.width() - 30, 14);
        p.drawText(emailRect, Qt::AlignVCenter | Qt::AlignLeft,
                   p.fontMetrics().elidedText(m_userEmail, Qt::ElideRight, int(emailRect.width())));
    }
}

void ProfileWidget::enterEvent(QEnterEvent *) { m_hovered = true; update(); }
void ProfileWidget::leaveEvent(QEvent *)      { m_hovered = false; update(); }
void ProfileWidget::mousePressEvent(QMouseEvent *) { emit clicked(); }

//=============================================================================
// Divider / SectionLabel
//=============================================================================
Divider::Divider(QWidget *parent) : QWidget(parent) { setFixedHeight(1); }

void Divider::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.fillRect(rect().adjusted(16, 0, -16, 0), QColor(60, 60, 65));
}

SectionLabel::SectionLabel(const QString &text, QWidget *parent)
    : QWidget(parent), m_text(text)
{
    setFixedHeight(32);
}

QSize SectionLabel::sizeHint() const { return QSize(200, 32); }

void SectionLabel::setCollapsed(bool collapsed)
{
    m_collapsed = collapsed;
    setVisible(!collapsed);
    update();
}

void SectionLabel::paintEvent(QPaintEvent *)
{
    if (m_collapsed) return;
    QPainter p(this);
    p.setRenderHint(QPainter::TextAntialiasing);

    QFont f = font();
    f.setPixelSize(10);
    f.setWeight(QFont::DemiBold);
    f.setLetterSpacing(QFont::AbsoluteSpacing, 1);
    p.setFont(f);
    p.setPen(QColor(110, 110, 115));
    p.drawText(rect().adjusted(20, 8, 0, 0), Qt::AlignTop | Qt::AlignLeft, m_text.toUpper());
}

//=============================================================================
// HoverSidebar Implementation
//=============================================================================
HoverSidebar::HoverSidebar(QWidget *parent)
    : QWidget(parent)
{
    setMouseTracking(true);
    setAttribute(Qt::WA_Hover);
    setFixedWidth(m_collapsedWidth);

    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setSpacing(0);
    m_mainLayout->setContentsMargins(0, 12, 0, 8);

    // ── Logo ──────────────────────────────────────────────────────────────
    auto *logoWidget = new QWidget(this);
    logoWidget->setFixedHeight(48);
    auto *logoLayout = new QHBoxLayout(logoWidget);
    logoLayout->setContentsMargins(8, 8, 8, 8);

    m_logoLabel = new QLabel("SF", this);
    m_logoLabel->setFixedSize(32, 32);
    m_logoLabel->setAlignment(Qt::AlignCenter);
    m_logoLabel->setStyleSheet(
        "QLabel { background: qlineargradient(x1:0,y1:0,x2:1,y2:1,"
        "stop:0 #1a5faa,stop:1 #0e3a6a); color:#fff; font-weight:800;"
        "font-size:12px; border-radius:8px; }");

    m_appNameLabel = new QLabel("SyntaxFlow", this);
    m_appNameLabel->setStyleSheet(
        "QLabel { color:#ddd; font-size:13px; font-weight:700;"
        "font-family:'Segoe UI',sans-serif; background:transparent; }");
    m_appNameLabel->setVisible(false);

    logoLayout->addWidget(m_logoLabel);
    logoLayout->addWidget(m_appNameLabel);
    logoLayout->addStretch();
    m_mainLayout->addWidget(logoWidget);

    // ── Main nav section ─────────────────────────────────────────────────
    auto *navSection = new SectionLabel("Navigation", this);
    m_sectionLabels.append(navSection);
    m_mainLayout->addWidget(navSection);

    auto *navContainer = new QWidget(this);
    m_navLayout = new QVBoxLayout(navContainer);
    m_navLayout->setSpacing(2);
    m_navLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->addWidget(navContainer);

    // ── Spacer ────────────────────────────────────────────────────────────
    m_mainLayout->addStretch();

    // ── Bottom section ────────────────────────────────────────────────────
    auto *bottomSection = new SectionLabel("Settings", this);
    m_sectionLabels.append(bottomSection);
    m_mainLayout->addWidget(bottomSection);

    auto *bottomContainer = new QWidget(this);
    m_bottomLayout = new QVBoxLayout(bottomContainer);
    m_bottomLayout->setSpacing(2);
    m_bottomLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->addWidget(bottomContainer);

    // ── Divider + Profile ─────────────────────────────────────────────────
    m_mainLayout->addWidget(new Divider(this));
    m_mainLayout->addSpacing(4);

    m_profile = new ProfileWidget(this);
    m_profile->setUserName("Developer");
    connect(m_profile, &ProfileWidget::clicked, this, &HoverSidebar::profileClicked);
    m_mainLayout->addWidget(m_profile);

    updateChildStates(true);
}

// ── Public API ────────────────────────────────────────────────────────────────

int HoverSidebar::addNavItem(const QString &iconText, const QString &label, const QColor &accent)
{
    auto *btn = new NavButton(iconText, label, this);
    btn->setAccentColor(accent);
    btn->setCollapsed(!m_isExpanded);

    const int index = m_navButtons.size();
    m_navButtons.append(btn);
    m_navLayout->addWidget(btn);

    connect(btn, &NavButton::clicked, this, [this, index]() {
        setSelectedIndex(index);
        emit navigationChanged(index);
    });

    if (m_selectedIndex == -1) setSelectedIndex(0);
    return index;
}

int HoverSidebar::addBottomItem(const QString &iconText, const QString &label, const QColor &accent)
{
    auto *btn = new NavButton(iconText, label, this);
    btn->setAccentColor(accent);
    btn->setCollapsed(!m_isExpanded);

    const int index = m_bottomButtons.size();
    m_bottomButtons.append(btn);
    m_bottomLayout->addWidget(btn);

    connect(btn, &NavButton::clicked, this, [this, index]() {
        emit bottomItemClicked(index);
    });

    return index;
}

void HoverSidebar::setNavBadge(int index, int count)
{
    if (index >= 0 && index < m_navButtons.size())
        m_navButtons[index]->setBadge(count);
}

void HoverSidebar::setUserName(const QString &name)   { m_profile->setUserName(name); }
void HoverSidebar::setUserEmail(const QString &email) { m_profile->setUserEmail(email); }
void HoverSidebar::setOnlineStatus(bool online)        { m_profile->setOnlineStatus(online); }

void HoverSidebar::setSelectedIndex(int index)
{
    if (index >= 0 && index < m_navButtons.size()) {
        for (int i = 0; i < m_navButtons.size(); ++i)
            m_navButtons[i]->setSelected(i == index);
        m_selectedIndex = index;
    }
}

void HoverSidebar::setCollapsedWidth(int width)
{
    m_collapsedWidth = width;
    if (!m_isExpanded) setFixedWidth(m_collapsedWidth);
}

void HoverSidebar::setExpandedWidth(int width)
{
    m_expandedWidth = width;
    if (m_isExpanded) setFixedWidth(m_expandedWidth);
}

void HoverSidebar::setExpanded(bool expanded)
{
    if (m_isExpanded == expanded) return;
    m_isExpanded = expanded;
    setFixedWidth(expanded ? m_expandedWidth : m_collapsedWidth);
    updateChildStates(!expanded);
    emit expandedChanged(expanded);
}

void HoverSidebar::updateChildStates(bool collapsed)
{
    for (NavButton *btn : m_navButtons)    btn->setCollapsed(collapsed);
    for (NavButton *btn : m_bottomButtons) btn->setCollapsed(collapsed);
    m_profile->setCollapsed(collapsed);
    if (m_appNameLabel) m_appNameLabel->setVisible(!collapsed);
    for (SectionLabel *s : m_sectionLabels) s->setCollapsed(collapsed);
}

void HoverSidebar::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.fillRect(rect(), QColor(30, 30, 35));
    p.setPen(QColor(50, 50, 55));
    p.drawLine(width() - 1, 0, width() - 1, height());
}

void HoverSidebar::enterEvent(QEnterEvent *) { setExpanded(true); raise(); }
void HoverSidebar::leaveEvent(QEvent *)      { setExpanded(false); }

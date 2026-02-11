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

QSize NavButton::sizeHint() const
{
    return QSize(200, 44);
}

void NavButton::setSelected(bool selected)
{
    if (m_selected != selected) {
        m_selected = selected;
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

void NavButton::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setRenderHint(QPainter::TextAntialiasing);

    QRectF r = rect().adjusted(6, 2, -6, -2);
    const qreal radius = 8;

    // Background
    QColor bgColor;
    if (m_selected) {
        bgColor = QColor(m_accentColor.red(), m_accentColor.green(),
                         m_accentColor.blue(), 40);
    } else if (m_pressed) {
        bgColor = QColor(255, 255, 255, 20);
    } else if (m_hovered) {
        bgColor = QColor(255, 255, 255, 12);
    } else {
        bgColor = Qt::transparent;
    }

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

    QColor iconColor = m_selected ? m_accentColor :
                           (m_hovered ? QColor(220, 220, 220) : QColor(160, 160, 160));
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
                               (m_hovered ? QColor(230, 230, 230) : QColor(180, 180, 180));
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
            badgeRect = QRectF(r.right() - 12, r.top() + 6, badgeWidth, 16);
        } else {
            badgeRect = QRectF(r.right() - badgeWidth - 8,
                               (r.height() - 16) / 2 + r.top(), badgeWidth, 16);
        }

        QPainterPath badgePath;
        badgePath.addRoundedRect(badgeRect, 8, 8);
        p.fillPath(badgePath, QColor(220, 70, 70));

        p.setPen(Qt::white);
        p.drawText(badgeRect, Qt::AlignCenter, badgeText);
    }
}

void NavButton::enterEvent(QEnterEvent *)
{
    m_hovered = true;
    update();
}

void NavButton::leaveEvent(QEvent *)
{
    m_hovered = false;
    m_pressed = false;
    update();
}

void NavButton::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_pressed = true;
        update();
    }
}

void NavButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_pressed = false;
        update();
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

void ProfileWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setRenderHint(QPainter::TextAntialiasing);

    QRectF r = rect().adjusted(6, 4, -6, -4);

    // Hover background
    if (m_hovered) {
        QPainterPath path;
        path.addRoundedRect(r, 8, 8);
        p.fillPath(path, QColor(255, 255, 255, 10));
    }

    // Avatar
    const qreal avatarSize = 36;
    QRectF avatarRect(r.left() + 8, (r.height() - avatarSize) / 2 + r.top(),
                      avatarSize, avatarSize);

    // Avatar background
    QPainterPath avatarPath;
    avatarPath.addEllipse(avatarRect);
    p.fillPath(avatarPath, QColor(80, 140, 200));

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
    if (initials.isEmpty() && !m_userName.isEmpty()) {
        initials = m_userName[0].toUpper();
    }
    p.drawText(avatarRect, Qt::AlignCenter, initials);

    // Online indicator
    const qreal indicatorSize = 10;
    QRectF indicatorRect(avatarRect.right() - indicatorSize + 2,
                         avatarRect.bottom() - indicatorSize + 2,
                         indicatorSize, indicatorSize);

    p.setPen(QPen(QColor(30, 30, 35), 2));
    p.setBrush(m_online ? QColor(80, 180, 100) : QColor(130, 130, 130));
    p.drawEllipse(indicatorRect);

    // User info (when expanded)
    if (!m_collapsed) {
        QFont nameFont = font();
        nameFont.setPixelSize(12);
        nameFont.setWeight(QFont::DemiBold);
        p.setFont(nameFont);
        p.setPen(QColor(220, 220, 220));

        QRectF nameRect(avatarRect.right() + 10, r.top() + 10,
                        r.width() - avatarRect.width() - 30, 16);
        p.drawText(nameRect, Qt::AlignVCenter | Qt::AlignLeft,
                   p.fontMetrics().elidedText(m_userName, Qt::ElideRight,
                                              int(nameRect.width())));

        QFont emailFont = font();
        emailFont.setPixelSize(10);
        p.setFont(emailFont);
        p.setPen(QColor(130, 130, 130));

        QRectF emailRect(avatarRect.right() + 10, nameRect.bottom() + 2,
                         r.width() - avatarRect.width() - 30, 14);
        p.drawText(emailRect, Qt::AlignVCenter | Qt::AlignLeft,
                   p.fontMetrics().elidedText(m_userEmail, Qt::ElideRight,
                                              int(emailRect.width())));
    }
}

void ProfileWidget::enterEvent(QEnterEvent *)
{
    m_hovered = true;
    update();
}

void ProfileWidget::leaveEvent(QEvent *)
{
    m_hovered = false;
    update();
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
    p.fillRect(rect().adjusted(16, 0, -16, 0), QColor(60, 60, 65));
}

//=============================================================================
// SectionLabel Implementation
//=============================================================================
SectionLabel::SectionLabel(const QString &text, QWidget *parent)
    : QWidget(parent)
    , m_text(text)
{
    setFixedHeight(32);
}

QSize SectionLabel::sizeHint() const
{
    return QSize(200, 32);
}

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

    QFont labelFont = font();
    labelFont.setPixelSize(10);
    labelFont.setWeight(QFont::DemiBold);
    labelFont.setLetterSpacing(QFont::AbsoluteSpacing, 1);
    p.setFont(labelFont);

    p.setPen(QColor(110, 110, 115));
    p.drawText(rect().adjusted(20, 8, 0, 0),
               Qt::AlignTop | Qt::AlignLeft, m_text.toUpper());
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

    // Main layout
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setSpacing(0);
    m_mainLayout->setContentsMargins(0, 12, 0, 8);

    // Logo area
    auto *logoWidget = new QWidget(this);
    logoWidget->setFixedHeight(40);
    m_mainLayout->addWidget(logoWidget);

    // Main section label
    auto *mainSection = new SectionLabel("Navigation", this);
    m_mainLayout->addWidget(mainSection);

    // Navigation container
    auto *navContainer = new QWidget(this);
    m_navLayout = new QVBoxLayout(navContainer);
    m_navLayout->setSpacing(2);
    m_navLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->addWidget(navContainer);

    // Create navigation buttons
    auto *btnExplorer = new NavButton("📁", "Explorer", this);
    auto *btnSearch = new NavButton("🔍", "Search", this);
    auto *btnGit = new NavButton("⌥", "Source Control", this);
    btnGit->setBadge(3);
    auto *btnDebug = new NavButton("▶", "Run & Debug", this);
    auto *btnExtensions = new NavButton("⊞", "Extensions", this);
    btnExtensions->setBadge(5);

    addNavButton(btnExplorer);
    addNavButton(btnSearch);
    addNavButton(btnGit);
    addNavButton(btnDebug);
    addNavButton(btnExtensions);

    // Set first as selected
    setSelectedIndex(0);

    // Spacer
    m_mainLayout->addStretch();

    // Bottom section
    auto *bottomSection = new SectionLabel("Settings", this);
    m_mainLayout->addWidget(bottomSection);

    // Settings button
    auto *btnSettings = new NavButton("⚙", "Settings", this);
    btnSettings->setAccentColor(QColor(160, 140, 200));
    addNavButton(btnSettings);

    // Divider
    auto *divider = new Divider(this);
    m_mainLayout->addWidget(divider);
    m_mainLayout->addSpacing(4);

    // Profile
    m_profile = new ProfileWidget(this);
    m_profile->setUserName("John Developer");
    m_profile->setUserEmail("john@company.dev");
    m_mainLayout->addWidget(m_profile);

    // Initial collapsed state
    updateChildStates(true);
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

void HoverSidebar::setExpanded(bool expanded)
{
    if (m_isExpanded != expanded) {
        m_isExpanded = expanded;
        setFixedWidth(expanded ? m_expandedWidth : m_collapsedWidth);
        updateChildStates(!expanded);
        emit expandedChanged(expanded);
    }
}

void HoverSidebar::updateChildStates(bool collapsed)
{
    for (NavButton *btn : m_navButtons) {
        btn->setCollapsed(collapsed);
    }
    m_profile->setCollapsed(collapsed);

    // Update section labels
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

    // Solid background
    p.fillRect(rect(), QColor(30, 30, 35));

    // Right border
    p.setPen(QColor(50, 50, 55));
    p.drawLine(width() - 1, 0, width() - 1, height());
}

void HoverSidebar::enterEvent(QEnterEvent *)
{
    setExpanded(true);
    raise();
}

void HoverSidebar::leaveEvent(QEvent *)
{
    setExpanded(false);
}

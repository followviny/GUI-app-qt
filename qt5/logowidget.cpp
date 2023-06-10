#include "logowidget.h"
#include <QPainter>

LogoWidget::LogoWidget(QWidget* parent)
    : QWidget(parent),
    m_logo(":/files/logo.jpg") {
}

void LogoWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    QImage image = m_logo.toImage();
    for(int x = int(image.width() * 0.47); x < int(image.width() * 0.6); x++) {
        for(int y = int(image.height() * 0.45); y < int(image.height() * 0.5); y++) {
            QColor color(0, 0, 0);
            image.setPixelColor(x, y, color);
        }
    }
    m_logo = QPixmap::fromImage(image);
    painter.drawPixmap(rect(), m_logo);
    painter.end();
}

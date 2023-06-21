#ifndef LOGOWIDGET_H
#define LOGOWIDGET_H

#include <QWidget>
#include <QPixmap>

class LogoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LogoWidget(QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QPixmap m_logo;
};

#endif // LOGOWIDGET_H

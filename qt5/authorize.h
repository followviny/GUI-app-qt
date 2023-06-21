#ifndef AUTHORIZE_H
#define AUTHORIZE_H

#include <QDialog>

namespace Ui
{
class Authorize;
}

class Authorize : public QDialog
{
    Q_OBJECT

public:
    explicit Authorize(QWidget *parent = nullptr);
    ~Authorize();

    bool isAuthorized() const;

private:
    void onOkButtonClicked();
    void onCheckBoxStateChanged(int arg1);
    Ui::Authorize *ui;
    bool authorized;
};

#endif // AUTHORIZE_H

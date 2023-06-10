#ifndef AUTHORIZE_H
#define AUTHORIZE_H

#include <QDialog>

namespace Ui {
class authorize;
}

class authorize : public QDialog
{
    Q_OBJECT

public:
    explicit authorize(QWidget *parent = nullptr);
    ~authorize();

    bool isAuthorized() const;

private slots:
    void on_okButton_clicked();

    void on_checkBox_stateChanged(int arg1);

private:
    Ui::authorize *ui;
    bool authorized;
};

#endif // AUTHORIZE_H

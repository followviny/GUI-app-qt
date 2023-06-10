#include "authorize.h"
#include "ui_authorize.h"
#include <QMessageBox>

authorize::authorize(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::authorize)
{
    ui->setupUi(this);
    ui->okButton->setProperty("objectName", "okButton");
    ui->lineEdit2->setEchoMode(QLineEdit::Password);
}

authorize::~authorize()
{
    delete ui;
}

bool authorize::isAuthorized() const
{
    return authorized;

}

void authorize::on_okButton_clicked()
{
    QString username = ui->lineEdit1->text();
    QString password = ui->lineEdit2->text();

    if (username == "user" && password == "123")
    {
        authorized = true;
        close();
    }
    else
    {
        QMessageBox::critical(this, "Error", "Invalid username or password");
    }
}


void authorize::on_checkBox_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked)
        ui->lineEdit2->setEchoMode(QLineEdit::Normal);
    else
        ui->lineEdit2->setEchoMode(QLineEdit::Password);

}


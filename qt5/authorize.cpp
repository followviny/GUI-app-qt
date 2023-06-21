#include "authorize.h"
#include "ui_authorize.h"
#include <QMessageBox>

Authorize::Authorize(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Authorize)
{
    ui->setupUi(this);
    connect(ui->okButton, &QPushButton::clicked, this, &Authorize::onOkButtonClicked);
    connect(ui->checkBox, &QCheckBox::stateChanged, this, &Authorize::onCheckBoxStateChanged);
    ui->lineEdit2->setEchoMode(QLineEdit::Password);
}

Authorize::~Authorize()
{
    delete ui;
}

bool Authorize::isAuthorized() const
{
    return authorized;

}

void Authorize::onOkButtonClicked()
{
    QString username = ui->lineEdit1->text();
    QString password = ui->lineEdit2->text();

    if (true) //login and password can be set, e.g. username == "user" && password == "123"
    {
        authorized = true;
        close();
    }
    else
    {
        QMessageBox::critical(this, "Error", "Invalid username or password");
    }
}


void Authorize::onCheckBoxStateChanged(int arg1)
{
    if (arg1 == Qt::Checked)
        ui->lineEdit2->setEchoMode(QLineEdit::Normal);
    else
        ui->lineEdit2->setEchoMode(QLineEdit::Password);

}


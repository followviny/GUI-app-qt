#include "datadialog.h"

#include "datadialog.h"
#include <QFormLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

static bool CheckNetworthPattern(const QString& str)
{
    static const QRegularExpression re("^\\$\\d+(\\.\\d+)? B$");
    return re.match(str).hasMatch();
}

DataDialog::DataDialog(const QString& option_name, Command cmd, const std::set<QString>& available_names, QWidget *parent)
    : QDialog(parent), available_names(available_names), cmd(cmd)
{
    setWindowTitle(option_name);

    auto formLayout = new QFormLayout(this);

    QStringList fields = {"name", "networth", "age", "country", "source", "industry"};
    if (cmd == DEL) {
        fields.resize(1);
    }
    for (int i = 0; i < fields.size(); ++i) {
        lineEdits.push_back(new QLineEdit(this));
        formLayout->addRow(fields[i], lineEdits[i]);
        if (fields[i] == "age") {
            lineEdits[i]->setValidator(new QRegularExpressionValidator(QRegularExpression("\\d+"), this));
        }
    }



    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    formLayout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &DataDialog::validateInput);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &DataDialog::reject);
}

QStringList DataDialog::getData() const
{
    QStringList result;
    for (const auto& lineEdit : lineEdits) {
        result << lineEdit->text();
    }
    return result;
}

void DataDialog::validateInput()
{
    int cnt = 0;
    for (const auto& lineEdit : lineEdits) {
        if (lineEdit->text().isEmpty()) {
            QMessageBox::critical(this, "Error", "All fields must be filled!");
            return;
        }
    }
    if (cmd != DEL) {
        if (!CheckNetworthPattern(lineEdits[1]->text())) {
            QMessageBox::critical(this, "Error", "networth does not match pattern `$<float> B`, e.g. `$107 B`, `$67.3 B`");
            return;
        }
        bool ok;
        int age = lineEdits[2]->text().toInt(&ok);
        if (!ok || age < 0 || age > 100) {
            QMessageBox::critical(this, "Error", "Age must be a number between 0 and 100.");
            return;
        }

    }
    if (cmd == EDIT) {
        if (!available_names.count(lineEdits[0]->text())) {
            QMessageBox::critical(this, "Error", "No such person.");
            return;
        }
    } else if (cmd == ADD){
        if (available_names.count(lineEdits[0]->text())) {
            QMessageBox::critical(this, "Error", "Such a person already exists.");
            return;
        }
    } else {
        if (!available_names.count(lineEdits[0]->text())) {
            QMessageBox::critical(this, "Error", "No such person.");
            return;
        }
    }



    accept();
}

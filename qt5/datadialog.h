#ifndef DATADIALOG_H
#define DATADIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <set>

class DataDialog : public QDialog
{
    Q_OBJECT

public:
    enum Command {
       ADD,
       EDIT,
       DEL
    };
    DataDialog(const QString& option_name, Command cmd, const std::set<QString>& available_names = {}, QWidget *parent = nullptr);
    ~DataDialog() { for (const auto& ptr : lineEdits) delete ptr;}

    QStringList getData() const;

private slots:
    void validateInput();

private:
    std::vector<QLineEdit*> lineEdits;
    const std::set<QString> available_names;
    Command cmd;
};

#endif // DATADIALOG_H

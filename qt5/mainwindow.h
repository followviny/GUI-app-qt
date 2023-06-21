#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "csv_table.h"
#include "logowidget.h"
#include "authorize.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    CSVTable* model;



    void onAddButtonClicked();
    void onEditButtonClicked();
    void onDelButtonClicked();
    void onLoadTableClicked();
    void onSaveTableClicked();
    void onSearchButtonClicked();
    void showLogoDialog();
    void inSortChanged(const QString& item);
    bool tableLoaded{false};
    LogoWidget* logoWidget;
};
#endif // MAINWINDOW_H


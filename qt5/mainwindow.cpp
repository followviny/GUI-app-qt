#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include "datadialog.h"
#include "QDebug"
#include "QAction"
#include "QVBoxLayout"
#include "QComboBox"
#include "authorize.h"
#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    hide();

    Authorize authWindow;
    authWindow.exec();

    if (authWindow.isAuthorized())
    {
        show();
    }
    else
    {
        QApplication::quit();
    }

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::onAddButtonClicked);
    connect(ui->EditButton, &QPushButton::clicked, this, &MainWindow::onEditButtonClicked);
    connect(ui->delButton, &QPushButton::clicked, this, &MainWindow::onDelButtonClicked);
    connect(ui->loadButton, &QPushButton::clicked, this, &MainWindow::onLoadTableClicked);
    connect(ui->saveButton, &QPushButton::clicked, this, &MainWindow::onSaveTableClicked);
    connect(ui->searchButton, &QPushButton::clicked, this, &MainWindow::onSearchButtonClicked);

    QStringList items = CSVTable::GetSortTypes();
    ui->comboBox->addItems(items);
    ui->comboBox->setCurrentIndex(0);

    connect(ui->comboBox, &QComboBox::currentTextChanged, this, &MainWindow::inSortChanged);

    QMenu* helpMenu = menuBar()->addMenu(tr("Help"));

    QAction* logo_act = new QAction(tr("Show Logo"), this);
    connect(logo_act, &QAction::triggered, this, &MainWindow::showLogoDialog);
    helpMenu->addAction(logo_act);

    logoWidget = new LogoWidget();
    logoWidget->resize(500, 500);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete model;
}

void MainWindow::onAddButtonClicked()
{
    if (!tableLoaded)
    {
        return;
    }

    auto dialog = new DataDialog("Add person", DataDialog::ADD, model->GetNames(), this);

    while (dialog->exec() == QDialog::Accepted)
    {
        auto data = dialog->getData();
        QString name = data[0];

        if (model->GetNames().count(name))
        {
            QMessageBox::critical(this, "Error", "A person with the same name already exists.");
        }
        else
        {
            model->AddData(data);
            break;
        }
    }
    dialog->deleteLater();

}


void MainWindow::onEditButtonClicked()
{
    if (!tableLoaded)
    {
        return;
    }
    auto dialog = new DataDialog("Edit person", DataDialog::EDIT, model->GetNames(), this);

    QModelIndexList selectedIndexes = ui->tableView->selectionModel()->selectedIndexes();
    if ((selectedIndexes.isEmpty()) || (selectedIndexes.size() > 1))
    {
        QMessageBox::information(this, "No selection", "Please select a person to edit.");
        dialog->deleteLater();
        return;
    }

    int selectedRow = selectedIndexes.first().row();
    QStringList rowData;
    for (int column = 1; column < model->columnCount(); ++column)
    {
        QModelIndex index = model->index(selectedRow, column);
        QString data = model->data(index).toString();
        rowData.append(data);
    }

    dialog->setData(rowData);
    dialog->setDisableNameField(true);
    if (dialog->exec() == QDialog::Accepted)
    {
        auto data = dialog->getData();
        model->AddData(data, true);
    }

    dialog->deleteLater();
}


void MainWindow::onDelButtonClicked()
{
    if (!tableLoaded)
    {
        return;
    }
    auto dialog = new DataDialog("Delete person", DataDialog::DEL, model->GetNames(), this);

    if (dialog->exec() == QDialog::Accepted)
    {
        auto data = dialog->getData();
        model->DelData(data[0]);
    }

    dialog->deleteLater();
}

void MainWindow::onLoadTableClicked()
{
    if (tableLoaded)
    {
        return;
    }
    QString fileName = QFileDialog::getOpenFileName(this, "Open CSV File", "", "CSV Files (*.csv)");
    if (!fileName.isEmpty())
    {
        model = new CSVTable(fileName);
        ui->tableView->setModel(model);
        tableLoaded = true;
    }
//    model = new CSVTable(":/files/dataset_1629.csv");

    ui->tableView->setModel(model);
}

void MainWindow::onSaveTableClicked()
{
    if (!tableLoaded)
    {
        return;
    }
    QString fileName = QFileDialog::getSaveFileName(this, "Save Table", "", "CSV Files (*.csv)");

    if (!fileName.isEmpty())
    {
        QFile file(fileName);

        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
        {
            QTextStream stream(&file);

            QStringList headerData;
            for (int column = 0; column < model->columnCount(); ++column)
            {
                headerData.append(model->headerData(column, Qt::Horizontal).toString());
            }
            stream << headerData.join(",") << "\n";

            for (int row = 0; row < model->rowCount(); ++row)
            {
                QStringList rowData;
                for (int column = 0; column < model->columnCount(); ++column)
                {
                    QModelIndex index = model->index(row, column);
                    QString data = model->data(index).toString();
                    rowData.append(data);
                }
                stream << rowData.join(",") << "\n";
            }

            file.close();
            QMessageBox::information(this, "Saved", "The table has been saved successfully.");
        } else
        {
            QMessageBox::critical(this, "Error", "Please try again.");
        }
    }

}

void MainWindow::showLogoDialog()
{
    QDialog* logoDialog = new QDialog(this);
    QVBoxLayout* layout = new QVBoxLayout(logoDialog);
    layout->addWidget(logoWidget);
    logoDialog->resize(500,300);
    logoDialog->setModal(true);
    logoDialog->show();

    connect(logoDialog, &QDialog::accepted, logoDialog, &QDialog::deleteLater);

}

void MainWindow::inSortChanged(const QString& item)
{
    if (!tableLoaded)
    {
        return;
    }
    model->SetSortSpecification(item);
    model->SortByCurrentSpecification();
}



void MainWindow::onSearchButtonClicked()
{
    if (!tableLoaded)
    {
        return;
    }


    QString searchName = ui->searchInput->text().toLower().trimmed();

    if (searchName.trimmed().isEmpty())
    {
        return;
    }

    for (int row = 0; row < model->rowCount(); ++row)
    {
        QModelIndex index = model->index(row, 1);
        QString name = model->data(index, Qt::DisplayRole).toString().toLower();
        if (name == searchName)
        {
            ui->tableView->selectRow(row);
            return;
        }
    }
    QMessageBox::information(this, "No result", "No such person.");

}




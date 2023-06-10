#include "simpletablemodel.h"

CSVTable::CSVTable(QObject* parent) : QAbstractTableModel(parent)
{
    // Добавляем некоторые данные для отображения в таблице
    for (int i = 0; i < 100; ++i) {
        std::vector<QString> row;
        for (int j = 0; j < 5; ++j) {
            row.push_back(QString("Cell %1 %2").arg(i).arg(j));
        }
        data_.push_back(row);
    }
}

int CSVTable::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return data_.size();
}

int CSVTable::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return data_[0].size();
}

QVariant CSVTable::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole)
        return data_[index.row()][index.column()];

    return QVariant();
}

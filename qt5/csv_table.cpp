#include "csv_table.h"
#include <QResource>
#include "csv_parser.h"
#include <QMessageBox>
#include <QDebug>
#include <QRegularExpression>
#include "csv_serializer.h"

CSVTable::CSVTable(const QString& csv_data_fname, QObject* parent) : QAbstractTableModel(parent), csv_data_fname_(csv_data_fname)
{
    std::vector<QStringList> csv;
    try {
        csv = parseFile(csv_data_fname_);
    } catch (std::exception& e) {
        QMessageBox::critical(nullptr, "An error occurred", e.what());
        return;
    }

    header_ = csv[0];
    for (int i = 1; i < csv.size(); ++i) {
        data_.push_back(csv[i]);
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
    return header_.size();
}

QVariant CSVTable::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole)
        return data_[index.row()][index.column()];

    return QVariant();
}

QVariant CSVTable::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        return header_[section];
    }
    return QVariant();
}

void CSVTable::AddData(QStringList data, bool replace)
{
    data.prepend("0");
    if (replace) {
        for (size_t i = 0; i < data_.size(); ++i) {
            if (data_[i][1] == data[1]) {
                data_[i] = data;
            }
        }
    } else {
        int newRowIndex = rowCount();

        beginInsertRows(QModelIndex(), newRowIndex, newRowIndex);

        data_.push_back(data);

        endInsertRows();
    }
    Resort();
}

static float GetFloatFromNetworth(const QString& s)
{
    const static QRegularExpression re("\\$(\\d+(\\.\\d+)?)[\\s]?B");
    QRegularExpressionMatch match = re.match(s);
    if (match.hasMatch()) {
        QString numberStr = match.captured(1);
        return numberStr.toFloat();
    }
    return 0;
}

void CSVTable::Resort()
{
    std::sort(data_.begin(), data_.end(), [](const QStringList& a, const QStringList& b) -> bool {
        return GetFloatFromNetworth(a[2]) > GetFloatFromNetworth(b[2]);
    });
    for (size_t i = 0; i < data_.size(); ++i) {
        data_[i][0] = QString::number(i + 1);
    }
    SortByCurrentSpecification();
}

void CSVTable::DelData(const QString& name)
{
    for (size_t i = 0; i < data_.size(); ++i) {
        if (data_[i][1] == name) {

            beginRemoveRows(QModelIndex(), i, i);
            data_.erase(data_.begin() + i);
            endRemoveRows();
            break;
        }
    }
    Resort();
}

std::set<QString> CSVTable::GetNames()
{
    std::set<QString> result;
    for (size_t i = 0; i < data_.size(); ++i) {
        result.insert(data_[i][1]);
    }
    return result;
}

void CSVTable::SetSortSpecification(const QString& item)
{
    sort_specification_ = item;
}

void CSVTable::SortByCurrentSpecification()
{
    if (sort_specification_ == "networth high to low") {
        std::sort(data_.begin(), data_.end(), [](const QStringList& a, const QStringList& b) -> bool {
            return GetFloatFromNetworth(a[2]) > GetFloatFromNetworth(b[2]);
        });
    } else if (sort_specification_ == "networth low to high")  {
        std::sort(data_.begin(), data_.end(), [](const QStringList& a, const QStringList& b) -> bool {
            return GetFloatFromNetworth(a[2]) < GetFloatFromNetworth(b[2]);
        });
    } else if (sort_specification_ == "age high to low") {
        std::sort(data_.begin(), data_.end(), [](const QStringList& a, const QStringList& b) -> bool {
            return a[3] > b[3];
        });
    } else if (sort_specification_ == "age low to high") {
        std::sort(data_.begin(), data_.end(), [](const QStringList& a, const QStringList& b) -> bool {
            return a[3] < b[3];
        });
    }
    int rows = rowCount();
    int columns = columnCount();
    QModelIndex topLeft = createIndex(0, 0);
    QModelIndex bottomRight = createIndex(rows - 1, columns - 1);
    emit dataChanged(topLeft, bottomRight);
}

void CSVTable::Save()
{
    SaveCSV(csv_data_fname_, data_);
}



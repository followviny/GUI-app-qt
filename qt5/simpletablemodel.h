#ifndef SIMPLETABLEMODEL_HPP
#define SIMPLETABLEMODEL_HPP

#include <QAbstractTableModel>
#include <vector>

class CSVTable : public QAbstractTableModel
{
    Q_OBJECT
public:
    CSVTable(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

private:
    std::vector<std::vector<QString>> data_;
    QStringList header_;
};

#endif // SIMPLETABLEMODEL_HPP

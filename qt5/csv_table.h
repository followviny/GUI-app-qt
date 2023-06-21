#ifndef CSV_TABLE_H
#define CSV_TABLE_H

#include <QAbstractTableModel>
#include <vector>
#include <set>

class CSVTable : public QAbstractTableModel
{
    Q_OBJECT
public:
    CSVTable(const QString& csv_data_fname, QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    void AddData(QStringList data, bool replace = false);
    void DelData(const QString& name);
    std::set<QString> GetNames();
    static QStringList GetSortTypes()
    {
        return {"networth high to low", "networth low to high", "age high to low", "age low to high"};
    }
    void SetSortSpecification(const QString& item);
    void SortByCurrentSpecification();
    void Save();


private:
    void Resort();
    std::vector<QStringList> data_;
    QStringList header_;
    const QString csvDataFname_;
    QString sortSpecification_{"networth high to low"};
};

#endif // CSV_TABLE_H

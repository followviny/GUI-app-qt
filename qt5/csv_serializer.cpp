#include "csv_parser.h"

#include <QFile>
#include <QTextStream>

void SaveCSV(const QString& fname, const std::vector<QStringList> data)
{
    QFile file(fname);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (const auto& line : data) {
            for (size_t i = 0; i < line.size(); ++i) {
                out << "\"";
                out << line[i];
                out << "\"";
                out << (i + 1 != line.size() ? "," : "\n");
            }
        }
        file.close();
    }
}


#include "csv_parser.h"
#include <QFile>
#include <exception>
#include <QTextStream>
#include <QResource>
#include <QDebug>

static QStringList ParseLine(const QString& line)
{
    QStringList result;
    QString value;
    bool inside = false;

    for (const auto& c : line)
    {
        if (c == '\"')
        {
            inside = !inside;
        }
        else if (c == ',' && !inside) //finished parsing a value
        {
            result.append(value);
            value.clear();
        }
        else
        {
            value += c;
        }
    }

    if (!value.isEmpty())
        result.append(value);

    return result;
}

std::vector<QStringList> parseFile(const QString& fname)
{
    QFile file(fname);

    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        throw std::runtime_error("Failed to open file.");
    }

    std::vector<QStringList> result;
    QTextStream stream(&file);

    while (!stream.atEnd())
    {
        result.emplace_back(ParseLine(stream.readLine())); //vect added to result
    }
    file.close();
    return result;
}


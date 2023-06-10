#ifndef CSV_PARSER_H
#define CSV_PARSER_H

#include <vector>
#include <QString>


std::vector<QStringList> parseFile(const QString& fname);

#endif // CSV_PARSER_H

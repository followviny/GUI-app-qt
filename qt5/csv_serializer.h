#ifndef CSV_SERIALIZER_H
#define CSV_SERIALIZER_H
#include "QString"

void SaveCSV(const QString& fname, const std::vector<QStringList> data);

#endif // CSV_SERIALIZER_H

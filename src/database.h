#ifndef DBHELPER_H
#define DBHELPER_H

#include <QDate>
#include <QJsonObject>
#include <QSqlRecord>
#include <QString>

namespace Database {
QString initConnection(const QString& owner);
void closeConnection(const QString& name);
QString dateToStringFilter(const QDate& date);
QJsonObject sqlRecordToJsonObject(const QSqlRecord& record);
};  // namespace Database

#endif  // DBHELPER_H

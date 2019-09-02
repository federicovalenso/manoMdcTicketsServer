#include "statisticsmodel.h"
#include <QJsonArray>
#include <QSqlRecord>

const QString StatisticsModel::kCountsByIntervalTemplate =
    "SELECT hour(created_at) as created_hour, count(id) as count FROM tickets "
    "WHERE DATE(created_at) >= %1 AND DATE(created_at) <= %2 GROUP BY "
    "created_hour";

QJsonDocument StatisticsModel::getCountsByInterval(const QDate &from,
                                                   const QDate &to) {
  using namespace Database;
  model_.setQuery(QString(kCountsByIntervalTemplate)
                      .arg(dateToStringFilter(from))
                      .arg(dateToStringFilter(to)),
                  database_);
  QJsonArray jsonArray;
  for (int i = 0; i < model_.rowCount(); ++i) {
    jsonArray.append(sqlRecordToJsonObject(model_.record(i)));
  }
  return QJsonDocument{jsonArray};
}

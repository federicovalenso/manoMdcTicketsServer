#include "statisticsmodel.h"
#include <QJsonArray>
#include <QSqlField>
#include <QSqlRecord>

CountByHour::CountByHour(QString &&created_at, QString &&count)
    : created_hour(created_at), count(count) {}

const QString StatisticsModel::kCountsByIntervalTemplate =
    "SELECT hour(created_at) as created_hour, count(id) as count FROM tickets "
    "WHERE DATE(created_at) >= %1 AND DATE(created_at) <= %2 GROUP BY "
    "created_hour";

std::vector<CountByHour> StatisticsModel::getCountsByInterval(const QDate &from,
                                                              const QDate &to) {
  using namespace Database;
  model_.setQuery(QString(kCountsByIntervalTemplate)
                      .arg(dateToStringFilter(from))
                      .arg(dateToStringFilter(to)),
                  database_);
  QJsonArray jsonArray;
  std::vector<CountByHour> result;
  result.reserve(static_cast<size_t>(model_.rowCount()));
  for (int i = 0; i < model_.rowCount(); ++i) {
    result.emplace_back(
        model_.record(i).field("created_hour").value().toString(),
        model_.record(i).field("count").value().toString());
  }
  return result;
}

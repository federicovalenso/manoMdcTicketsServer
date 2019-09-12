#ifndef STATISTICSMODEL_H
#define STATISTICSMODEL_H

#include <QDate>
#include <QJsonDocument>
#include <QSqlQueryModel>
#include "database.h"

struct CountByHour {
  QString created_hour;
  QString count;

  CountByHour(QString&& created_hour, QString&& count);
};

class StatisticsModel {
 public:
  std::vector<CountByHour> getCountsByInterval(const QDate& from,
                                               const QDate& to);

 private:
  static const QString kCountsByIntervalTemplate;

  QSqlQueryModel model_;
  const QSqlDatabase database_ =
      QSqlDatabase::database(Database::initConnection("StatisticsModel"));
};

#endif  // STATISTICSMODEL_H

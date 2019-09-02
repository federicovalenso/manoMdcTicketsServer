#ifndef STATISTICSMODEL_H
#define STATISTICSMODEL_H

#include <QDate>
#include <QJsonDocument>
#include <QSqlQueryModel>
#include "database.h"

class StatisticsModel {
 public:
  QJsonDocument getCountsByInterval(const QDate& from, const QDate& to);

 private:
  static const QString kCountsByIntervalTemplate;

  QSqlQueryModel model_;
  const QSqlDatabase database_ =
      QSqlDatabase::database(Database::initConnection("StatisticsModel"));
};

#endif  // STATISTICSMODEL_H

#ifndef DATABASEMIGRATOR_H
#define DATABASEMIGRATOR_H

#include "serversettings.h"

#include <QFile>
#include <QSqlError>
#include <QSqlQuery>

class DatabaseMigrator {
 public:
  DatabaseMigrator();
  ~DatabaseMigrator();
  void migrate();

 private:
  ServerSettings& settings_ = ServerSettings::getInstance();
  QSqlQuery* query_;
};

#endif  // DATABASEMIGRATOR_H

#ifndef DATABASEMIGRATOR_H
#define DATABASEMIGRATOR_H

#include <QFile>
#include <QSqlError>
#include <QSqlQuery>

class DatabaseMigrator {
 public:
  DatabaseMigrator();
  ~DatabaseMigrator();
  void migrate();

 private:
  QSqlQuery* query_;
};

#endif  // DATABASEMIGRATOR_H

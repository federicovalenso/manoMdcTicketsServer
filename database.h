#ifndef DBHELPER_H
#define DBHELPER_H

#include "serversettings.h"

#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QString>

class Database {
 public:
  static const QString DB_TYPE;

  Database(const QString& owner);
  ~Database();

  bool startTransaction() noexcept;
  bool commitTransaction() noexcept;
  bool isValid() const;
  QString connectionName() const;

 private:
  const QString CONNECTION_NAME_PREFIX = "mano";

  ServerSettings& settings_ = ServerSettings::getInstance();
  QString connection_name_;
  bool is_valid_;

  void setConnection() noexcept;
};

#endif  // DBHELPER_H

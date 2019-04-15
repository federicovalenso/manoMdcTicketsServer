#include "database.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QStringList>
#include <QThread>
#include <QtSql>

Database::Database(const QString& owner)
    : connection_name_(
          CONNECTION_NAME_PREFIX + owner +
          QString::number((long long)QThread::currentThreadId(), 16)) {
  setConnection();
}

Database::~Database() {}

bool Database::startTransaction() noexcept {
  return QSqlDatabase::database(connection_name_).transaction();
}

bool Database::commitTransaction() noexcept {
  return QSqlDatabase::database(connection_name_).commit();
}

void Database::setConnection() noexcept {
  QSqlDatabase db = QSqlDatabase::database(connection_name_, false);
  if (db.isValid() == true) {
    is_valid_ = true;
  } else {
    db = QSqlDatabase::addDatabase(DB_TYPE, connection_name_);
    db.setHostName(settings_.getServerAddr());
    db.setDatabaseName(settings_.getBaseName());
    db.setUserName(settings_.getUserName());
    db.setPassword(settings_.getPassword());
    is_valid_ = db.open();
  }
}

bool Database::isValid() const { return is_valid_; }

QString Database::connectionName() const { return connection_name_; }

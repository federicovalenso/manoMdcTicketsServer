#include "databasemigrator.h"
#include "database.h"

#include <QDebug>
#include <QDir>
#include <QSqlError>

#include <algorithm>
#include <stdexcept>

DatabaseMigrator::DatabaseMigrator() {
  QSqlDatabase db = QSqlDatabase::addDatabase(Database::DB_TYPE, "migrator");
  db.setHostName(settings_.getServerAddr());
  db.setPort(settings_.getPort());
  db.setUserName(settings_.getUserName());
  db.setPassword(settings_.getPassword());
  if (!db.open()) {
    throw std::runtime_error(db.lastError().databaseText().toUtf8());
  }
  query_ = new QSqlQuery(db);
}

DatabaseMigrator::~DatabaseMigrator() {
  QSqlDatabase::removeDatabase("migrator");
}

void DatabaseMigrator::migrate() {
  const QString migrations_dir(":/migrations/etc/migrations");
  QDir dir(migrations_dir, "", QDir::SortFlag::Name, QDir::Filter::Files);
  for (const auto& filename : dir.entryList()) {
    QFile file(migrations_dir + '/' + filename);
    if (!file.open(QIODevice::ReadOnly)) {
      throw std::invalid_argument("Can't open file: " + filename.toUtf8());
    }
    if (!query_->exec(file.readAll())) {
      throw std::runtime_error(query_->lastError().databaseText().toUtf8());
    }
  }
}

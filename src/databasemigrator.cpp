#include "databasemigrator.h"
#include "database.h"
#include "serversettings.h"

#include <QCoreApplication>
#include <QDir>
#include <QSqlError>
#include <QVariant>

#include <algorithm>
#include <stdexcept>

DatabaseMigrator::DatabaseMigrator() {
  auto app = qApp;
  QSqlDatabase db = QSqlDatabase::addDatabase(
      app->property(Settings::kDbType).toString(), "migrator");
  db.setHostName(app->property(Settings::kServerAddress).toString());
  db.setPort(app->property(Settings::kPort).toInt());
  db.setUserName(app->property(Settings::kUserName).toString());
  db.setPassword(app->property(Settings::kPassword).toString());
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

#include "database.h"
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlField>
#include <QSqlTableModel>
#include <QThread>
#include "serversettings.h"

QString Database::initConnection(const QString& owner) {
  const auto connection_name =
      owner + QString::number((long long)QThread::currentThreadId(), 16);
  QSqlDatabase db = QSqlDatabase::database(connection_name, false);
  if (!db.isValid()) {
    auto app = qApp;
    db = QSqlDatabase::addDatabase(app->property(Settings::kDbType).toString(),
                                   connection_name);
    db.setHostName(app->property(Settings::kServerAddress).toString());
    db.setPort(app->property(Settings::kPort).toInt());
    db.setDatabaseName(Settings::kBaseName);
    db.setUserName(app->property(Settings::kUserName).toString());
    db.setPassword(app->property(Settings::kPassword).toString());
    if (!db.open())
      throw std::runtime_error("Can't handle connection to database");
  }
  return connection_name;
}

QString Database::dateToStringFilter(const QDate& date) {
  return QString("STR_TO_DATE('%1', '%d/%m/%Y')")
      .arg(date.toString("dd/MM/yyyy"));
}

QJsonObject Database::sqlRecordToJsonObject(QSqlRecord&& record) {
  QJsonObject result;
  for (int i = 0; i < record.count(); ++i) {
    result.insert(record.fieldName(i), record.field(i).value().toString());
  }
  return result;
}

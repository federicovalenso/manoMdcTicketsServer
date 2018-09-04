#include "database.h"

#include <QStringList>
#include <QtSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QThread>
#include <QDebug>

Database::Database(const QString& owner):
    mConnectionName(CONNECTION_NAME_PREFIX +
                    owner +
                    QString::number((long long)QThread::currentThreadId(), 16))
{
    setConnection();
}

Database::~Database()
{
}

bool Database::startTransaction() noexcept
{
    return QSqlDatabase::database(mConnectionName).transaction();
}

bool Database::commitTransaction() noexcept
{
    return QSqlDatabase::database(mConnectionName).commit();
}

void Database::setConnection() noexcept
{
    QSqlDatabase db = QSqlDatabase::database(mConnectionName, false);
    if (db.isValid() == true) {
        mIsValid = true;
    } else {
        db = QSqlDatabase::addDatabase(DB_TYPE, mConnectionName);
        db.setHostName(mSettings.getServerAddr());
        db.setDatabaseName(mSettings.getBaseName());
        db.setUserName(mSettings.getUserName());
        db.setPassword(mSettings.getPassword());
        mIsValid = db.open();
    }
}

bool Database::isValid() const
{
    return mIsValid;
}

QString Database::connectionName() const
{
    return mConnectionName;
}

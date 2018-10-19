#ifndef DBHELPER_H
#define DBHELPER_H

#include "appsettings.h"

#include <QString>
#include <QSqlDatabase>
#include <QSqlTableModel>

class Database
{
public:
    Database(const QString& owner);
    ~Database();

    bool startTransaction() noexcept;
    bool commitTransaction() noexcept;
    bool isValid() const;
    QString connectionName() const;

private:
    const QString DB_TYPE = "QMYSQL";
    const QString CONNECTION_NAME_PREFIX = "mano";

    AppSettings& settings_ = AppSettings::getInstance();
    QString connection_name_;
    bool is_valid_;

    void setConnection() noexcept;
};

#endif // DBHELPER_H

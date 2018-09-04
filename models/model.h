#ifndef MODEL_H
#define MODEL_H

#include <QDateTime>
#include <QString>
#include <QJsonObject>
#include <QVariant>
#include <QSet>
#include <QMap>

#include "database.h"
#include "entities/entity.h"

using TableColumns = QSet<QString>;
using TableOptions = QMap<QString, QVariant>;

class Model
{
public:
    Model(const QString& owner);
    virtual ~Model();
    bool isValid();

    static QString getCurrentTime()
    {
        return QDateTime::currentDateTime().toString("yyyy-MM-dd HH-mm-ss");
    }

    static const QString ID_COL;
    static const QString CREATED_AT_COL;
    static const QString UPDATED_AT_COL;

protected:
    const QString mTableName;
    TableColumns mColumns;
    Database mDb;
};

#endif // MODEL_H

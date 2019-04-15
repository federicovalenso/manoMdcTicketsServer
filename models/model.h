#ifndef MODEL_H
#define MODEL_H

#include <optional>

#include <QDateTime>
#include <QJsonObject>
#include <QMap>
#include <QSet>
#include <QString>
#include <QVariant>

#include "database.h"
#include "entities/entity.h"

using TableColumns = QSet<QString>;
using TableOptions = QMap<QString, QVariant>;

class Model {
 public:
  Model(const QString& owner);
  virtual ~Model() = default;
  bool isValid();

  static QString getCurrentTime() {
    return QDateTime::currentDateTime().toString("yyyy-MM-dd HH-mm-ss");
  }

  static const QString ID_COL;
  static const QString CREATED_AT_COL;
  static const QString UPDATED_AT_COL;
  static const QByteArray ID_COL_PARAM;

 protected:
  const QString table_name_;
  TableColumns columns_;
  Database database_;
};

#endif  // MODEL_H

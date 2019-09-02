#ifndef MODEL_H
#define MODEL_H

#include <QDateTime>
#include <QJsonObject>
#include <QMap>
#include <QSet>
#include <QString>
#include <QVariant>
#include <optional>
#include "entities/entity.h"

using TableColumns = QSet<QString>;
using TableOptions = QMap<QString, QVariant>;

class Model {
 public:
  Model(const QString& owner);
  virtual ~Model() = default;

  static QString getCurrentTime() {
    return QDateTime::currentDateTime().toString("yyyy-MM-dd HH-mm-ss");
  }

  static const QString ID_COL;
  static const QString CREATED_AT_COL;
  static const QString UPDATED_AT_COL;
  static const QByteArray ID_COL_PARAM;

 protected:
  TableColumns columns_;
  const QString kConnectionName;
};

#endif  // MODEL_H

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
  virtual ~Model();

  static QString getCurrentTime() {
    return QDateTime::currentDateTime().toString("yyyy-MM-dd HH-mm-ss");
  }

  static const QString kIdCol;
  static const QString kCreatedAtCol;
  static const QString kUpdatedAtCol;
  static const QByteArray kIdColParam;

 protected:
  TableColumns columns_;
  const QString kConnectionName;
};

#endif  // MODEL_H

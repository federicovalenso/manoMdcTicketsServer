#ifndef TICKETACTIONMODEL_H
#define TICKETACTIONMODEL_H

#include <QJsonArray>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QVector>
#include <memory>
#include "entities/ticketaction.h"
#include "model.h"

class TicketActionModel : public Model {
 public:
  static const QString kTableName;
  static const QString kNameCol;
  static const QString kPrefixCol;

  TicketActionModel();
  ~TicketActionModel() = default;

  std::optional<TicketAction> getByName(const QString& name) noexcept;
  QVector<TicketAction> getAll();
  QVector<TicketAction> getByNamesArray(const QJsonArray& names);

 private:
  std::unique_ptr<QSqlTableModel> model_;
};

#endif  // TICKETACTIONMODEL_H

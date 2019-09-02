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
  static const QString TABLE_NAME;
  static const QString NAME_COL;
  static const QString PREFIX_COL;

  TicketActionModel();
  ~TicketActionModel() = default;

  std::optional<TicketAction> getByName(const QString& name) noexcept;
  QVector<TicketAction> getAll();
  QVector<TicketAction> getByNamesArray(const QJsonArray& names);

 private:
  std::unique_ptr<QSqlTableModel> model_;
};

#endif  // TICKETACTIONMODEL_H

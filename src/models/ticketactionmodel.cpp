#include "ticketactionmodel.h"
#include <QSqlField>

using namespace std;

const QString TicketActionModel::kTableName = "ticketActions";
const QString TicketActionModel::kNameCol = "action_name";
const QString TicketActionModel::kPrefixCol = "prefix";

TicketActionModel::TicketActionModel() : Model("TicketActionModel") {
  columns_.insert(kNameCol);
  model_ = std::make_unique<QSqlTableModel>(
      nullptr, QSqlDatabase::database(kConnectionName));
  model_->setTable(kTableName);
}

QVector<TicketAction> TicketActionModel::getAll() {
  QVector<TicketAction> result;
  model_->setFilter("");
  model_->select();
  for (int i = 0; i < model_->rowCount(); i++) {
    auto record = model_->record(i);
    TicketAction ticketAction;
    ticketAction.id = record.field(kIdCol).value().toInt();
    ticketAction.name = record.field(kNameCol).value().toString();
    ticketAction.prefix = record.field(kPrefixCol).value().toString();
    result.push_back(std::move(ticketAction));
  }
  return result;
}

optional<TicketAction> TicketActionModel::getByName(
    const QString& name) noexcept {
  QString filter = QString("%1='%2'").arg(kNameCol).arg(name);
  model_->setFilter(filter);
  model_->select();
  if (model_->rowCount() == 1) {
    auto record = model_->record(0);
    TicketAction result;
    result.id = record.field(kIdCol).value().toInt();
    result.name = record.field(kNameCol).value().toString();
    result.prefix = record.field(kPrefixCol).value().toString();
    return move(result);
  }
  return nullopt;
}

QVector<TicketAction> TicketActionModel::getByNamesArray(
    const QJsonArray& names = {}) {
  QString filter;
  for (const auto& name : names) {
    QString nameCondition =
        QString("%1='%2' AND ").arg(kNameCol).arg(name.toString());
    filter.append(nameCondition);
  }
  if (filter.size() > 0) {
    filter = filter.left(filter.size() - 5);
  }
  model_->setFilter(filter);
  model_->select();
  QVector<TicketAction> result;
  if (model_->rowCount() > 0) {
    for (int i = 0; i < model_->rowCount(); i++) {
      auto record = model_->record(i);
      TicketAction ticketAction;
      ticketAction.id = record.field(kIdCol).value().toInt();
      ticketAction.name = record.field(kNameCol).value().toString();
      ticketAction.prefix = record.field(kPrefixCol).value().toString();
      result.push_back(std::move(ticketAction));
    }
  }
  return result;
}

#include "ticketactionmodel.h"
#include <QSqlField>

using namespace std;

const QString TicketActionModel::TABLE_NAME = "ticketActions";
const QString TicketActionModel::NAME_COL = "action_name";
const QString TicketActionModel::PREFIX_COL = "prefix";

TicketActionModel::TicketActionModel() : Model("TicketActionModel") {
  columns_.insert(TicketActionModel::NAME_COL);
  model_ = std::make_unique<QSqlTableModel>(
      nullptr, QSqlDatabase::database(database_.connectionName()));
  model_->setTable(TicketActionModel::TABLE_NAME);
}

TicketActionModel::~TicketActionModel() {
  model_.reset();
  QSqlDatabase::database(database_.connectionName()).close();
  QSqlDatabase::removeDatabase(database_.connectionName());
}

QVector<TicketAction> TicketActionModel::getAll() {
  QVector<TicketAction> result;
  model_->setFilter("");
  model_->select();
  for (int i = 0; i < model_->rowCount(); i++) {
    auto record = model_->record(i);
    TicketAction ticketAction;
    ticketAction.id = record.field(Model::ID_COL).value().toInt();
    ticketAction.name =
        record.field(TicketActionModel::NAME_COL).value().toString();
    ticketAction.prefix =
        record.field(TicketActionModel::PREFIX_COL).value().toString();
    result.push_back(std::move(ticketAction));
  }
  return result;
}

optional<TicketAction> TicketActionModel::getByName(
    const QString& name) noexcept {
  QString filter =
      QString("%1='%2'").arg(TicketActionModel::NAME_COL).arg(name);
  model_->setFilter(filter);
  model_->select();
  if (model_->rowCount() == 1) {
    auto record = model_->record(0);
    TicketAction result;
    result.id = record.field(Model::ID_COL).value().toInt();
    result.name = record.field(TicketActionModel::NAME_COL).value().toString();
    result.prefix =
        record.field(TicketActionModel::PREFIX_COL).value().toString();
    return move(result);
  }
  return nullopt;
}

QVector<TicketAction> TicketActionModel::getByNamesArray(
    const QJsonArray& names = {}) {
  QString filter;
  for (const auto& name : names) {
    QString nameCondition = QString("%1='%2' AND ")
                                .arg(TicketActionModel::NAME_COL)
                                .arg(name.toString());
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
      ticketAction.id = record.field(Model::ID_COL).value().toInt();
      ticketAction.name =
          record.field(TicketActionModel::NAME_COL).value().toString();
      result.push_back(std::move(ticketAction));
    }
  }
  return result;
}

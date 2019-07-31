#include "ticketmodel.h"
#include <QSqlError>
#include <QSqlField>
#include <QSqlQuery>
#include <QSqlRelation>
#include <QVector>
#include <algorithm>
#include <stdexcept>
#include "entities/ticketaction.h"
#include "usermodel.h"
#ifdef QT_DEBUG
#include <QDebug>
#endif

using namespace std;

const QString TicketModel::TABLE_NAME = "tickets";
const QString TicketModel::NUMBER_BY_ACTION_COL = "number_by_action";
const QString TicketModel::ACTION_ID_COL = "action_id";
const QString TicketModel::USER_ID_COL = "user_id";
const QString TicketModel::ON_SERVICE_COL = "on_service";
const QString TicketModel::IS_MANUAL_COL = "is_manual";
const QString TicketModel::IS_DONE_COL = "is_done";
const QString TicketModel::IS_VOICED_COL = "is_voiced";
const QString TicketModel::WINDOW_NUMBER_COL = "window";
const QByteArray TicketModel::ON_SERVICE_PARAM = ON_SERVICE_COL.toUtf8();
const QByteArray TicketModel::IS_MANUAL_PARAM = IS_MANUAL_COL.toUtf8();
const QByteArray TicketModel::IS_DONE_PARAM = IS_DONE_COL.toUtf8();
const QByteArray TicketModel::IS_VOICED_PARAM = IS_VOICED_COL.toUtf8();
const QByteArray TicketModel::WINDOW_NUMBER_PARAM = WINDOW_NUMBER_COL.toUtf8();

QString fromBool(bool value) { return value == true ? "1" : "0"; }

TicketModel::TicketModel() : Model("TicketModel") {
  columns_.insert(NUMBER_BY_ACTION_COL);
  columns_.insert(ACTION_ID_COL);
  columns_.insert(USER_ID_COL);
  columns_.insert(ON_SERVICE_COL);
  columns_.insert(IS_DONE_COL);
  columns_.insert(IS_VOICED_COL);
  columns_.insert(IS_MANUAL_COL);
  columns_.insert(WINDOW_NUMBER_COL);
  columns_.insert(TicketActionModel::NAME_COL);
  model_ = std::make_unique<QSqlRelationalTableModel>(
      nullptr, QSqlDatabase::database(database_.connectionName()));
  model_->setTable(TABLE_NAME);
  model_->setRelation(
      ACTION_ID_COL_NUMBER,
      QSqlRelation(TicketActionModel::TABLE_NAME, TicketActionModel::ID_COL,
                   TicketActionModel::NAME_COL));
  model_->setEditStrategy(QSqlTableModel::EditStrategy::OnRowChange);
}

TicketModel::~TicketModel() {
  model_.reset();
  QSqlDatabase::database(database_.connectionName()).close();
  QSqlDatabase::removeDatabase(database_.connectionName());
}

optional<Ticket> TicketModel::save(const TableOptions &options) noexcept {
  QSqlRecord record;
  for (auto it = options.cbegin(); it != options.cend(); it++) {
    if (!columns_.contains(it.key())) {
      return nullopt;
    }
    QSqlField field(it.key());
    field.setValue(it.value());
    record.append(move(field));
  }
  if (model_->insertRecord(-1, record)) {
    return extractFromRecord(model_->record(model_->rowCount() - 1));
  }
  return nullopt;
}

QVector<Ticket> TicketModel::getAll() noexcept {
  QVector<Ticket> result;
  model_->setFilter("");
  model_->select();
  for (int i = 0; i < model_->rowCount(); i++) {
    result.push_back(extractFromRecord(model_->record(i)));
  }
  return result;
}

QVector<Ticket> TicketModel::getAvailableTickets(bool on_service,
                                                 bool is_manual) noexcept {
  QVector<Ticket> result;
  QString filter = QString("DATE(%1) = CURDATE() AND %2 = %3")
                       .arg(CREATED_AT_COL)
                       .arg(ON_SERVICE_COL)
                       .arg(fromBool(on_service));
  if (!is_manual) {
    filter.append(QString(" AND %1 = 0").arg(IS_MANUAL_COL));
  }
  model_->setFilter(filter);
  model_->select();
  for (int i = 0; i < model_->rowCount(); i++) {
    result.push_back(extractFromRecord(model_->record(i)));
  }
  return result;
}

optional<Ticket> TicketModel::getOldestNonVoicedTicket() noexcept {
  QString filter = QString("DATE(%1) = CURDATE() AND %2='0' AND %3='1'")
                       .arg(CREATED_AT_COL)
                       .arg(IS_VOICED_COL)
                       .arg(ON_SERVICE_COL);
  QString actionsFilter;
  model_->setSort(CREATED_AT_COL_NUMBER, Qt::SortOrder::AscendingOrder);
  model_->setFilter(filter);
  model_->select();
  Ticket result;
  int rowCount = model_->rowCount();
  if (rowCount > 0) {
    return extractFromRecord(model_->record(0));
  };
  return nullopt;
}

optional<Ticket> TicketModel::updateTicket(
    const TableOptions &options) noexcept {
  if (validateOptions(options)) {
    QString filter = QString("%1.%2='%3'")
                         .arg(TABLE_NAME)
                         .arg(ID_COL)
                         .arg(options.value(ID_COL).toString());
    model_->setFilter(filter);
    model_->select();
#ifdef QT_DEBUG
    qDebug() << model_->query().lastError();
    qDebug() << model_->query().lastQuery();
#endif
    if (model_->rowCount() == 1) {
      QSqlRecord record;
      for (auto it = options.cbegin(); it != options.cend(); it++) {
        QSqlField field(it.key());
        field.setValue(it.value());
        record.append(field);
      }
      model_->setRecord(0, record);
      if (model_->submit()) {
        return extractFromRecord(model_->record(0));
      }
    }
  }
  return nullopt;
}

bool TicketModel::validateOptions(const TableOptions &options) const {
  bool result = false;
  if (options.size() > 0) {
    size_t errors = 0;
    for (auto it = options.cbegin(); it != options.cend(); ++it) {
      if (!columns_.contains(it.key())) {
        ++errors;
        break;
      }
    }
    if (errors == 0) result = true;
  }
  return result;
}

Ticket TicketModel::extractFromRecord(const QSqlRecord &record) const {
  Ticket ticket;
  ticket.id = record.field(ID_COL).value().toInt();
  ticket.number_by_action = record.field(NUMBER_BY_ACTION_COL).value().toInt();
  ticket.action = record.field(TicketActionModel::NAME_COL).value().toString();
  ticket.user_id = record.field(USER_ID_COL).value().toInt();
  ticket.created_at =
      record.field(CREATED_AT_COL).value().toDateTime().toSecsSinceEpoch();
  ticket.updated_at =
      record.field(UPDATED_AT_COL).value().toDateTime().toSecsSinceEpoch();
  ticket.on_service = record.field(ON_SERVICE_COL).value().toBool();
  ticket.is_done = record.field(IS_DONE_COL).value().toBool();
  ticket.is_voiced = record.field(IS_VOICED_COL).value().toBool();
  ticket.is_manual = record.field(IS_MANUAL_COL).value().toBool();
  ticket.window_number = record.field(WINDOW_NUMBER_COL).value().toInt();
  return ticket;
}

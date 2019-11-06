#include "ticketmodel.h"
#include <QSqlError>
#include <QSqlField>
#include <QSqlQuery>
#include <QSqlRelation>
#include <QVector>
#include <algorithm>
#include <stdexcept>
#include "database.h"
#include "entities/ticketaction.h"
#include "usermodel.h"
#ifdef QT_DEBUG
#include <QDebug>
#endif

using namespace std;

const QString TicketModel::kTableName = "tickets";
const QString TicketModel::kNumberByActionCol = "number_by_action";
const QString TicketModel::kTicketIdCol = TicketModel::kTableName + ".id";
const QString TicketModel::kActionIdCol = "action_id";
const QString TicketModel::kUserIdCol = "user_id";
const QString TicketModel::kOnServiceCol = "on_service";
const QString TicketModel::kIsManualCol = "is_manual";
const QString TicketModel::kIsDoneCol = "is_done";
const QString TicketModel::kIsVoicedCol = "is_voiced";
const QString TicketModel::kWindowNumberCol = "window";
const QString TicketModel::kTicketNumber = "ticket_number";
const QByteArray TicketModel::kOnServiceParam =
    TicketModel::kOnServiceCol.toUtf8();
const QByteArray TicketModel::kIsManualParam =
    TicketModel::kIsManualCol.toUtf8();
const QByteArray TicketModel::kIsDoneParam = TicketModel::kIsDoneCol.toUtf8();
const QByteArray TicketModel::kIsVoicedParam =
    TicketModel::kIsVoicedCol.toUtf8();
const QByteArray TicketModel::kWindowNumberParam =
    TicketModel::kWindowNumberCol.toUtf8();

QString fromBool(bool value) { return value == true ? "1" : "0"; }

TicketModel::TicketModel() : Model("TicketModel") {
  columns_.insert(kNumberByActionCol);
  columns_.insert(kActionIdCol);
  columns_.insert(kUserIdCol);
  columns_.insert(kOnServiceCol);
  columns_.insert(kIsDoneCol);
  columns_.insert(kIsVoicedCol);
  columns_.insert(kIsManualCol);
  columns_.insert(kWindowNumberCol);
  columns_.insert(TicketActionModel::kNameCol);
  model_ = std::make_unique<QSqlRelationalTableModel>(
      nullptr, QSqlDatabase::database(kConnectionName));
  model_->setTable(kTableName);
  model_->setRelation(
      kActionIdColNumber,
      QSqlRelation(TicketActionModel::kTableName, TicketActionModel::kIdCol,
                   TicketActionModel::kNameCol));
  model_->setEditStrategy(QSqlTableModel::EditStrategy::OnRowChange);
}

optional<QJsonObject> TicketModel::save(const TableOptions &options) noexcept {
  QSqlRecord record;
  for (auto it = options.cbegin(); it != options.cend(); it++) {
    if (!columns_.contains(it.key())) {
      return nullopt;
    }
    QSqlField field(it.key());
    field.setValue(it.value());
    record.append(field);
  }
  if (model_->insertRecord(-1, record)) {
    return extractFromRecord(model_->record(model_->rowCount() - 1),
                             ExtractionModes::INSERT);
  }
  return nullopt;
}

QVector<QJsonObject> TicketModel::getAll() noexcept {
  QVector<QJsonObject> result;
  model_->setFilter("");
  model_->select();
  for (int i = 0; i < model_->rowCount(); i++) {
    result.push_back(extractFromRecord(model_->record(i)));
  }
  return result;
}

QVector<QJsonObject> TicketModel::getAvailableTickets(bool on_service,
                                                      bool is_manual) noexcept {
  QVector<QJsonObject> result;
  QString filter = QString("DATE(%1) = CURDATE() AND %2 = %3")
                       .arg(kCreatedAtCol)
                       .arg(kOnServiceCol)
                       .arg(fromBool(on_service));
  if (!is_manual) {
    filter.append(QString(" AND %1 = 0").arg(kIsManualCol));
  }
  model_->setFilter(filter);
  model_->select();
  for (int i = 0; i < model_->rowCount(); i++) {
    result.push_back(extractFromRecord(model_->record(i)));
  }
  return result;
}

optional<QJsonObject> TicketModel::getOldestNonVoicedTicket() noexcept {
  return getByFilter(QString("DATE(%1) = CURDATE() AND %2='0' AND %3='1'")
                         .arg(kCreatedAtCol)
                         .arg(kIsVoicedCol)
                         .arg(kOnServiceCol));
}

std::optional<QJsonObject> TicketModel::getById(int id) noexcept {
  return getByFilter(QString("%1=%2").arg(kTicketIdCol).arg(id));
}

std::optional<QJsonObject> TicketModel::getByFilter(QString filter) noexcept {
  model_->setSort(kCreatedAtColNumber, Qt::SortOrder::AscendingOrder);
  model_->setFilter(filter);
  model_->select();
  Ticket result;
  int rowCount = model_->rowCount();
  if (rowCount > 0) {
    return extractFromRecord(model_->record(0));
  }
  return nullopt;
}

optional<QJsonObject> TicketModel::updateTicket(
    const TableOptions &options) noexcept {
  if (validateOptions(options)) {
    QString filter = QString("%1.%2='%3'")
                         .arg(kTableName)
                         .arg(kIdCol)
                         .arg(options.value(kIdCol).toString());
    model_->setFilter(filter);
    model_->select();
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

QJsonObject TicketModel::extractFromRecord(const QSqlRecord &record,
                                           ExtractionModes mode) const {
  QJsonObject result;
  result.insert(kIdCol, record.field(kIdCol).value().toInt());
  result.insert(kNumberByActionCol,
                record.field(kNumberByActionCol).value().toInt());
  const auto actionFieldName = mode == ExtractionModes::INSERT
                                   ? kActionIdCol
                                   : TicketActionModel::kNameCol;
  const auto actionName = record.field(actionFieldName).value().toString();
  result.insert(TicketActionModel::kNameCol, actionName);
  auto ticketModel = TicketActionModel{}.getByName(actionName);
  result.insert(kTicketNumber,
                ticketModel->prefix +
                    QString::number(result.value(kNumberByActionCol).toInt()));
  result.insert(kUserIdCol, record.field(kUserIdCol).value().toInt());
  result.insert(
      kCreatedAtCol,
      record.field(kCreatedAtCol).value().toDateTime().toSecsSinceEpoch());
  result.insert(
      kUpdatedAtCol,
      record.field(kUpdatedAtCol).value().toDateTime().toSecsSinceEpoch());
  result.insert(kOnServiceCol, record.field(kOnServiceCol).value().toBool());
  result.insert(kIsDoneCol, record.field(kIsDoneCol).value().toBool());
  result.insert(kIsVoicedCol, record.field(kIsVoicedCol).value().toBool());
  result.insert(kIsManualCol, record.field(kIsManualCol).value().toBool());
  result.insert(kWindowNumberCol,
                record.field(kWindowNumberCol).value().toInt());
  return result;
}

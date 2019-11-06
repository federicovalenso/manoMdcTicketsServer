#include "ticket.h"

#include "models/ticketactionmodel.h"
#include "models/ticketmodel.h"

QJsonObject Ticket::toQJsonObject() const {
  QJsonObject result;
  result.insert(TicketModel::kIdCol, id);
  result.insert(TicketModel::kCreatedAtCol, created_at);
  result.insert(TicketModel::kUpdatedAtCol, updated_at);
  result.insert(TicketActionModel::kNameCol, action);
  if (const auto ticketAction = TicketActionModel{}.getByName(action);
      ticketAction) {
  }
  auto ticketNumber = [this]() -> std::optional<QString> {
    if (const auto ticketAction = TicketActionModel{}.getByName(action);
        ticketAction) {
      return ticketAction->prefix + QString::number(number_by_action);
    } else {
      return std::nullopt;
    }
  }();
  if (ticketNumber) {
    result.insert(TicketModel::kTicketNumber, std::move(*ticketNumber));
  }
  result.insert(TicketModel::kUserIdCol, user_id);
  result.insert(TicketModel::kOnServiceCol, on_service);
  result.insert(TicketModel::kIsDoneCol, is_done);
  result.insert(TicketModel::kIsVoicedCol, is_voiced);
  result.insert(TicketModel::kIsManualCol, is_manual);
  result.insert(TicketModel::kWindowNumberCol, window_number);
  return result;
}

QJsonDocument Ticket::toQJsonDocument() const {
  return QJsonDocument(toQJsonObject());
}

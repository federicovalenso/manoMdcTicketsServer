#include <QDateTime>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSqlField>

#include <rules>
#include "entities/ticketaction.h"
#include "entities/user.h"
#include "models/ticketactionmodel.h"
#include "models/ticketmodel.h"
#include "models/usermodel.h"
#include "requestmapper.h"
#include "ticketcontroller.h"
#include "ticketcounter.h"
#include "usercontroller.h"

using namespace std;
using namespace stefanfrings;

const QByteArray TicketController::ACTION_PARAM = "action";
const QString TicketController::TICKET_NUMBER = "ticket_number";
const QString TicketController::WINDOW_NUMBER = "window_number";

inline bool toBool(const QByteArray &value) { return value == "1"; }

TicketController::TicketController() {}

QVariant getLoggedInUserId(HttpRequest &request, HttpResponse &response) {
  auto session =
      RequestMapper::sessionStore->getSession(request, response, false);
  return session.get(UserController::SESSION_PARAM_USER_ID);
}

void TicketController::index(HttpRequest &request, HttpResponse &response) {
  response.setHeader("Content-Type", "application/json");
  if (validateIndexRequest(request)) {
    auto parameters = request.getParameterMap();
    QByteArray on_service = parameters.value(TicketModel::ON_SERVICE_PARAM);
    QByteArray is_manual = parameters.value(TicketModel::IS_MANUAL_PARAM);
    QJsonArray jsonArr;
    for (const auto &ticket : TicketModel().getAvailableTickets(
             toBool(on_service), toBool((is_manual)))) {
      QJsonObject jsonObject;
      jsonObject.insert(TicketModel::ID_COL, ticket.id);
      jsonObject.insert(TicketModel::CREATED_AT_COL, ticket.created_at);
      jsonObject.insert(TicketActionModel::NAME_COL, ticket.action);
      jsonObject.insert(TicketModel::IS_MANUAL_COL, ticket.is_manual);
      auto ticketAction = TicketActionModel().getByName(ticket.action);
      jsonObject.insert(
          TICKET_NUMBER,
          ticketAction->prefix + QString::number(ticket.number_by_action));
      jsonArr.append(jsonObject);
    }
    QJsonDocument jsonDoc(jsonArr);
    response.write(jsonDoc.toJson(QJsonDocument::JsonFormat::Compact), true);
  } else {
    setClientError(response);
  }
}

void TicketController::store(HttpRequest &request, HttpResponse &response) {
  response.setHeader("Content-Type", "application/json");
  if (auto user_id = getLoggedInUserId(request, response); user_id.isNull()) {
    setUnauthorizedError(response);
    return;
  }
  QByteArray action = request.getParameter(ACTION_PARAM);
  if (auto ticketAction = TicketActionModel().getByName(action); ticketAction) {
    TableOptions options;
    options.insert(TicketModel::ACTION_ID_COL, ticketAction->id);
    options.insert(TicketModel::CREATED_AT_COL, TicketModel::getCurrentTime());
    if (auto ticket = TicketModel().save(options); ticket) {
      QJsonObject jsonObject;
      jsonObject.insert(
          TICKET_NUMBER,
          ticketAction->prefix + QString::number(ticket->number_by_action));
      jsonObject.insert(ACTION_PARAM, QString(action));
      jsonObject.insert(TicketModel::IS_VOICED_COL, ticket->is_voiced);
      QJsonDocument doc(jsonObject);
      response.write(doc.toJson(), true);
      ++TicketCounter::getInstance();
    } else {
      setServerError(response);
    }
  } else {
    setClientError(response);
  }
}

void TicketController::show(HttpRequest &request, HttpResponse &response) {
  response.setHeader("Content-Type", "application/json");
  if (auto user_id = getLoggedInUserId(request, response); user_id.isNull()) {
    setUnauthorizedError(response);
    return;
  }
  TicketModel ticketModel;
  if (auto ticket = ticketModel.getOldestNonVoicedTicket(); ticket) {
    QJsonObject jsonObject;
    jsonObject.insert(TicketModel::ID_COL, ticket->id);
    auto ticketAction = TicketActionModel().getByName(ticket->action);
    jsonObject.insert(
        TICKET_NUMBER,
        ticketAction->prefix + QString::number(ticket->number_by_action));
    jsonObject.insert(WINDOW_NUMBER, ticket->window_number);
    QJsonDocument doc(jsonObject);
    response.write(doc.toJson(), true);
  } else {
    response.write(QJsonDocument(QJsonObject()).toJson(), true);
  }
}

void TicketController::update(HttpRequest &request, HttpResponse &response) {
  response.setHeader("Content-Type", "application/json");
  auto user_id = getLoggedInUserId(request, response);
  if (user_id.isNull()) {
    setUnauthorizedError(response);
    return;
  }
  auto params = request.getParameterMap();
  auto id = params.value(TicketModel::ID_COL_PARAM).toInt();
  if (std::lock_guard g{m_};
      find(cbegin(locked_ticket_ids_), cend(locked_ticket_ids_), id) !=
      cend(locked_ticket_ids_)) {
    response.setStatus(409, "Conflict");
    return;
  } else {
    locked_ticket_ids_.push_back(id);
  }
  if (!validateUpdateRequest(request)) {
    setClientError(response);
    return;
  }
  TicketModel ticketModel;
  TableOptions options;
  options.insert(TicketModel::USER_ID_COL, user_id.toInt());
  for (auto it = params.cbegin(); it != params.cend(); ++it) {
    options.insert(it.key(), it.value());
  }
  if (ticketModel.updateTicket(options)) {
    response.setStatus(200);
  } else {
    setClientError(response);
  }
  {
    lock_guard g{m_};
    locked_ticket_ids_.erase(
        find(cbegin(locked_ticket_ids_), cend(locked_ticket_ids_), id));
  }
}

bool TicketController::validateIndexRequest(HttpRequest &request) noexcept {
  auto parameters = request.getParameterMap();
  QByteArray on_service = parameters.value(TicketModel::ON_SERVICE_PARAM);
  QByteArray is_manual = parameters.value(TicketModel::IS_MANUAL_PARAM);
  return Validate(
      IfRule<EmptyValueRule, AlwaysTrueRule, BoolRule>(
          EmptyValueRule(on_service), AlwaysTrueRule(), BoolRule(on_service)),
      IfRule<EmptyValueRule, AlwaysTrueRule, BoolRule>(
          EmptyValueRule(is_manual), AlwaysTrueRule(), BoolRule(is_manual)));
}

bool TicketController::validateUpdateRequest(HttpRequest &request) noexcept {
  auto parameters = request.getParameterMap();
  IntRule checkOnServiceValue(parameters.value(TicketModel::ON_SERVICE_PARAM),
                              0);
  IntRule checkIsDoneValue(parameters.value(TicketModel::IS_DONE_PARAM), 0);
  return Validate(
      IdRule(parameters.value(TicketModel::ID_COL_PARAM)),
      BoolRule(parameters.value(TicketModel::ON_SERVICE_PARAM)),
      BoolRule(parameters.value(TicketModel::IS_DONE_PARAM)),
      BoolRule(parameters.value(TicketModel::IS_VOICED_PARAM)),
      BoolRule(parameters.value(TicketModel::IS_MANUAL_PARAM)),
      WindowRule(parameters.value(TicketModel::WINDOW_NUMBER_PARAM)),
      IfRule<IntRule, IntRule, AlwaysTrueRule>(
          checkOnServiceValue, checkIsDoneValue, AlwaysTrueRule()));
}

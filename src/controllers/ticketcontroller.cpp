#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSqlField>

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

const QByteArray TicketController::kAction = "action";
const QString TicketController::kWindowNumber = "window_number";

inline bool toBool(const QByteArray &value) { return value == "1"; }

QVariant getLoggedInUserId(HttpRequest &request, HttpResponse &response) {
  auto session =
      RequestMapper::sessionStore->getSession(request, response, false);
  return session.get(UserController::kUserId);
}

void TicketController::index(HttpRequest &request,
                             HttpResponse &response) const {
  response.setHeader("Content-Type", "application/json");
  if (validateIndexRequest(request)) {
    auto parameters = request.getParameterMap();
    QByteArray on_service = parameters.value(TicketModel::kOnServiceParam);
    QByteArray is_manual = parameters.value(TicketModel::kIsManualParam);
    QJsonArray jsonArr;
    for (const auto &ticket : TicketModel().getAvailableTickets(
             toBool(on_service), toBool((is_manual)))) {
      jsonArr.append(ticket);
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
  QByteArray action = request.getParameter(kAction);
  TableOptions options;
  if (const auto ticketAction = TicketActionModel{}.getByName(action);
      ticketAction) {
    options.insert(TicketModel::kActionIdCol, ticketAction->id);
    options.insert(TicketModel::kCreatedAtCol, Model::getCurrentTime());
    if (auto ticket = TicketModel().save(options); ticket) {
      response.write(QJsonDocument(*ticket).toJson(), true);
      ++TicketCounter::instance();
    } else {
      setServerError(response);
    }
  } else {
    setServerError(response);
  }
}

void TicketController::show(HttpRequest &request,
                            HttpResponse &response) const {
  response.setHeader("Content-Type", "application/json");
  if (auto user_id = getLoggedInUserId(request, response); user_id.isNull()) {
    setUnauthorizedError(response);
    return;
  }
  TicketModel ticketModel;
  if (auto ticket = ticketModel.getOldestNonVoicedTicket(); ticket) {
    response.write(QJsonDocument(*ticket).toJson(), true);
  } else {
    response.write(QJsonDocument(QJsonObject()).toJson(), true);
  }
}

void TicketController::voiceTicket(HttpRequest &request,
                                   HttpResponse &response) const {
  response.setHeader("Content-Type", "application/json");
  if (getLoggedInUserId(request, response).isNull()) {
    setUnauthorizedError(response);
    return;
  }
  auto id = request.getParameter(TicketModel::kIdColParam);
  auto is_voiced = request.getParameter(TicketModel::kIsVoicedParam);
  if (!IdRule(id) || !BoolRule(is_voiced)) {
    setClientError(response);
    return;
  }
  if (const auto ticket = TicketModel().updateTicket(
          {{TicketModel::kIdCol, id}, {TicketModel::kIsVoicedCol, is_voiced}});
      ticket) {
    response.setStatus(200);
    response.write(QJsonDocument(*ticket).toJson(), true);
  } else {
    setClientError(response);
  }
}

void TicketController::takeTicket(HttpRequest &request,
                                  HttpResponse &response) const {
  response.setHeader("Content-Type", "application/json");
  auto user_id = getLoggedInUserId(request, response);
  if (user_id.isNull()) {
    setUnauthorizedError(response);
    return;
  }
  auto id = request.getParameter(TicketModel::kIdColParam);
  auto on_service = request.getParameter(TicketModel::kOnServiceParam);
  auto window_number = request.getParameter(TicketModel::kWindowNumberParam);
  if (!IdRule(id) || !BoolRule(on_service) || !WindowRule(window_number)) {
    setClientError(response);
    return;
  }
  lock_guard guard{ids_locker_};
  TicketModel ticketModel;
  if (auto ticket = ticketModel.getById(id.toInt()); ticket) {
    if (ticket->value(TicketModel::kOnServiceCol).toBool() &&
        on_service.toInt() == 1) {
      response.setStatus(409, "Conflict");
      return;
    }
  } else {
    setClientError(response);
    return;
  }
  if (const auto ticket = TicketModel().updateTicket(
          {{TicketModel::kIdCol, id},
           {TicketModel::kUserIdCol, user_id},
           {TicketModel::kOnServiceCol, on_service},
           {TicketModel::kWindowNumberCol, window_number}});
      ticket) {
    response.setStatus(200);
    response.write(QJsonDocument(*ticket).toJson(), true);
  } else {
    setClientError(response);
  }
}

void TicketController::returnTicket(HttpRequest &request,
                                    HttpResponse &response) const {
  response.setHeader("Content-Type", "application/json");
  auto user_id = getLoggedInUserId(request, response);
  if (user_id.isNull()) {
    setUnauthorizedError(response);
    return;
  }
  auto id = request.getParameter(TicketModel::kIdColParam);
  if (!IdRule(id)) {
    setClientError(response);
    return;
  }
  TicketModel ticketModel;
  if (auto ticket = ticketModel.getById(id.toInt()); ticket) {
    if (ticket->value(TicketModel::kUserIdCol).toInt() != user_id) {
      setClientError(response);
      return;
    }
  } else {
    setClientError(response);
    return;
  }
  if (const auto ticket =
          TicketModel().updateTicket({{TicketModel::kIdCol, id},
                                      {TicketModel::kOnServiceCol, 0},
                                      {TicketModel::kIsManualCol, 1}});
      ticket) {
    response.setStatus(200);
    response.write(QJsonDocument(*ticket).toJson(), true);
  } else {
    setClientError(response);
  }
}

void TicketController::finishTicket(HttpRequest &request,
                                    HttpResponse &response) const {
  response.setHeader("Content-Type", "application/json");
  auto user_id = getLoggedInUserId(request, response);
  if (user_id.isNull()) {
    setUnauthorizedError(response);
    return;
  }
  auto id = request.getParameter(TicketModel::kIdColParam);
  if (!IdRule(id)) {
    setClientError(response);
    return;
  }
  TicketModel ticketModel;
  if (auto ticket = ticketModel.getById(id.toInt()); ticket) {
    if (ticket->value(TicketModel::kUserIdCol).toInt() != user_id) {
      setClientError(response);
      return;
    }
  } else {
    setClientError(response);
    return;
  }
  if (const auto ticket =
          ticketModel.updateTicket({{TicketModel::kIdCol, id},
                                    {TicketModel::kIsVoicedCol, 1},
                                    {TicketModel::kOnServiceCol, 1},
                                    {TicketModel::kIsDoneCol, 1}});
      ticket) {
    response.setStatus(200);
    response.write(QJsonDocument(*ticket).toJson(), true);
  } else {
    setClientError(response);
  }
}

bool TicketController::validateIndexRequest(HttpRequest &request) const
    noexcept {
  auto on_service = request.getParameter(TicketModel::kOnServiceParam);
  auto is_manual = request.getParameter(TicketModel::kIsManualParam);
  return Validate(
      IfRule<EmptyValueRule, TrueRule, BoolRule>(
          EmptyValueRule(on_service), TrueRule(), BoolRule(on_service)),
      IfRule<EmptyValueRule, TrueRule, BoolRule>(
          EmptyValueRule(is_manual), TrueRule(), BoolRule(is_manual)));
}

bool TicketController::validateUpdateRequest(HttpRequest &request) const
    noexcept {
  auto parameters = request.getParameterMap();
  IntRule checkOnServiceValue(parameters.value(TicketModel::kOnServiceParam),
                              0);
  IntRule checkIsDoneValue(parameters.value(TicketModel::kIsDoneParam), 0);
  return Validate(IdRule(parameters.value(TicketModel::kIdColParam)),
                  BoolRule(parameters.value(TicketModel::kOnServiceParam)),
                  BoolRule(parameters.value(TicketModel::kIsDoneParam)),
                  BoolRule(parameters.value(TicketModel::kIsVoicedParam)),
                  BoolRule(parameters.value(TicketModel::kIsManualParam)),
                  WindowRule(parameters.value(TicketModel::kWindowNumberParam)),
                  IfRule<IntRule, IntRule, TrueRule>(
                      checkOnServiceValue, checkIsDoneValue, TrueRule()));
}

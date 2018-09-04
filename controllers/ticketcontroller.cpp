#include <stdexcept>
#include <QIntValidator>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDateTime>
#include <QSqlField>
#include <QDebug>
#include "ticketcontroller.h"
#include "models/ticketactionmodel.h"
#include "models/ticketmodel.h"
#include "models/usermodel.h"
#include "entities/ticket.h"
#include "entities/ticketaction.h"
#include "entities/user.h"

using namespace stefanfrings;

const QByteArray TicketController::USER_NAME_PARAM = "user_name";
const QByteArray TicketController::ACTION_PARAM = "action";
const QString TicketController::TICKET_NUMBER = "ticket_number";
const QString TicketController::WINDOW_NUMBER = "window_number";

TicketController::TicketController()
{}

void TicketController::index(HttpRequest& request, HttpResponse& response)
{
    response.setHeader("Content-Type", "application/json");
    auto params = request.getParameterMap();
    auto search = params.find(TicketModel::ON_SERVICE_COL.toUtf8());
    if (search != params.end()) {
        if (search.value().toInt() == 0) {
            QJsonArray jsonArr;
            for (const auto& ticket : TicketModel().getNonServicedTickets()) {
                QJsonObject jsonObject;
                jsonObject.insert(TicketModel::ID_COL, ticket.id);
                jsonObject.insert(TicketModel::CREATED_AT_COL, ticket.created_at);
                jsonObject.insert(TicketActionModel::NAME_COL, ticket.action);
                TicketAction ticketAction = TicketActionModel().getByName(ticket.action);
                jsonObject.insert(TICKET_NUMBER, ticketAction.prefix + QString::number(ticket.number_by_action));
                jsonArr.append(std::move(jsonObject));
            }
            QJsonDocument jsonDoc(jsonArr);
            response.write(jsonDoc.toJson(QJsonDocument::JsonFormat::Compact));
        }
    } else {
        setClientError(response);
    }
}

void TicketController::store(HttpRequest& request, HttpResponse& response)
{
    response.setHeader("Content-Type", "application/json");
    QByteArray action = request.getParameter(ACTION_PARAM);
    TicketAction ticketAction = TicketActionModel().getByName(action);
    if (ticketAction.id != -1) {
        TicketModel ticket_model;
        TableOptions options;
        options.insert(TicketModel::ACTION_ID_COL, ticketAction.id);
        options.insert(TicketModel::CREATED_AT_COL, TicketModel::getCurrentTime());
        Ticket ticket = ticket_model.save(options);
        if (ticket.isValid() == false) {
            setServerError(response);
        } else {
            QJsonObject jsonObject;
            jsonObject.insert(TICKET_NUMBER, ticketAction.prefix + QString::number(ticket.number_by_action));
            jsonObject.insert(ACTION_PARAM, QString(action));
            QJsonDocument doc(std::move(jsonObject));
            response.write(doc.toJson());
        }
    } else {
        setClientError(response);
    }
}

void TicketController::show(HttpRequest&, HttpResponse& response)
{
    response.setHeader("Content-Type", "application/json");
    TicketModel ticketModel;
    Ticket ticket = ticketModel.getOldestNonVoicedTicket();
    if (ticket.isValid()) {
        QJsonObject jsonObject;
        jsonObject.insert(TicketModel::ID_COL, ticket.id);
        TicketAction ticketAction = TicketActionModel().getByName(ticket.action);
        jsonObject.insert(TICKET_NUMBER, ticketAction.prefix + QString::number(ticket.number_by_action));
        jsonObject.insert(WINDOW_NUMBER, ticket.window_number);
        QJsonDocument doc(std::move(jsonObject));
        response.write(doc.toJson());
    } else {
        response.setStatus(400, "No tickets...");
    }
}

void TicketController::update(HttpRequest& request, HttpResponse& response)
{
    response.setHeader("Content-Type", "application/json");
    if (validateRequest(request) == true) {
        auto params = request.getParameterMap();
        User user = UserModel().getByName(request.getParameter(USER_NAME_PARAM));
        if (user.id != -1) {
            TicketModel ticketModel;
            TableOptions options;
            options.insert(TicketModel::USER_ID_COL, user.id);
            for (auto it = params.cbegin(); it != params.cend(); it++) {
                if (it.key() != USER_NAME_PARAM) {
                    options.insert(it.key(), it.value());
                }
            }
            Ticket ticket = ticketModel.updateTicket(options);
            if (ticket.isValid()) {
                response.setStatus(200);
            } else {
                setClientError(response);
            }
        } else {
            setClientError(response);
        }
    } else {
      setClientError(response);
    }
}

bool TicketController::validateRequest(HttpRequest &request)
{
    bool result = false;
    int pos = 0;
    auto params = request.getParameterMap();
    QByteArray id_param = TicketModel::ID_COL.toUtf8();
    QByteArray on_service_param = TicketModel::ON_SERVICE_COL.toUtf8();
    QByteArray is_done_param = TicketModel::IS_DONE_COL.toUtf8();
    QByteArray window_param = TicketModel::WINDOW_NUMBER_COL.toUtf8();
    QIntValidator id_validator;
    params.value(id_param);
    id_validator.setBottom(1);
    if (params.contains(id_param) == true) {
        QString id(params.value(id_param));
        if (id_validator.validate(id, pos) == QValidator::State::Acceptable) {
            QIntValidator action_validator(0, 1);
            QIntValidator window_validator(1, 8);
            if (params.contains(on_service_param) && params.contains(is_done_param) && params.contains(window_param)) {
                QString on_service(params.value(on_service_param));
                QString is_done(params.value(is_done_param));
                QString window(params.value(window_param));
                if (action_validator.validate(on_service, pos) == QValidator::State::Acceptable &&
                      action_validator.validate(is_done, pos) == QValidator::State::Acceptable &&
                        window_validator.validate(window, pos) == QValidator::State::Acceptable) {
                  if (on_service == "0") {
                      if (is_done == "0") {
                          result = true;
                      }
                  } else {
                      result = true;
                  }
                }
            }
        }
    }
    return result;
}

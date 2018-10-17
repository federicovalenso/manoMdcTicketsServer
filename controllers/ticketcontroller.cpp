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
#include "validators/requestvalidator.h"
#include "validators/rules/validationrule.h"
#include "validators/rules/intrule.h"
#include "validators/rules/idrule.h"
#include "validators/rules/ifrule.h"
#include "validators/rules/boolrule.h"
#include "validators/rules/emptyvaluerule.h"
#include "validators/rules/windowrule.h"

using namespace stefanfrings;

const QByteArray TicketController::USER_NAME_PARAM = "user_name";
const QByteArray TicketController::ACTION_PARAM = "action";
const QString TicketController::TICKET_NUMBER = "ticket_number";
const QString TicketController::WINDOW_NUMBER = "window_number";

bool toBool(const QByteArray& value)
{
    return value == "1";
}

TicketController::TicketController()
{}

void TicketController::index(HttpRequest& request, HttpResponse& response)
{
    response.setHeader("Content-Type", "application/json");
    if (validateIndexRequest(request) == true) {
        auto parameters = request.getParameterMap();
        QByteArray on_service = parameters.value(TicketModel::ON_SERVICE_PARAM);
        QByteArray is_manual = parameters.value(TicketModel::IS_MANUAL_PARAM);
        QJsonArray jsonArr;
        for (const auto& ticket : TicketModel().getAvailableTickets(toBool(on_service), toBool((is_manual)))) {
            QJsonObject jsonObject;
            jsonObject.insert(TicketModel::ID_COL, ticket.id);
            jsonObject.insert(TicketModel::CREATED_AT_COL, ticket.created_at);
            jsonObject.insert(TicketActionModel::NAME_COL, ticket.action);
            jsonObject.insert(TicketModel::IS_MANUAL_COL, ticket.is_manual);
            TicketAction ticketAction = TicketActionModel().getByName(ticket.action);
            jsonObject.insert(TICKET_NUMBER, ticketAction.prefix + QString::number(ticket.number_by_action));
            jsonArr.append(std::move(jsonObject));
        }
        QJsonDocument jsonDoc(jsonArr);
        response.write(jsonDoc.toJson(QJsonDocument::JsonFormat::Compact));
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
            jsonObject.insert(TicketModel::IS_VOICED_COL, ticket.is_voiced);
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
        response.write(QJsonDocument(QJsonObject()).toJson());
    }
}

void TicketController::update(HttpRequest& request, HttpResponse& response)
{
    response.setHeader("Content-Type", "application/json");
    if (validateUpdateRequest(request) == true) {
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

bool TicketController::validateIndexRequest(HttpRequest& request)
{
    auto parameters = request.getParameterMap();
    RequestValidator validator;
    QByteArray on_service = parameters.value(TicketModel::ON_SERVICE_PARAM);
    QByteArray is_manual = parameters.value(TicketModel::IS_MANUAL_PARAM);
    validator.AddRule(IfRule<EmptyValueRule, AlwaysTrueRule, BoolRule>(
                          EmptyValueRule(on_service),
                          AlwaysTrueRule(),
                          BoolRule(on_service)))
            .AddRule(IfRule<EmptyValueRule, AlwaysTrueRule, BoolRule>(
                         EmptyValueRule(is_manual),
                         AlwaysTrueRule(),
                         BoolRule(is_manual)));
    return validator.Validate();
}

bool TicketController::validateUpdateRequest(HttpRequest& request)
{
    auto parameters = request.getParameterMap();
    RequestValidator validator;
    IntRule checkOnServiceValue(parameters.value(TicketModel::ON_SERVICE_PARAM), 0);
    IntRule checkIsDoneValue(parameters.value(TicketModel::IS_DONE_PARAM), 0);
    validator.AddRule(IdRule(parameters.value(TicketModel::ID_COL_PARAM)))
            .AddRule(BoolRule(parameters.value(TicketModel::ON_SERVICE_PARAM)))
            .AddRule(BoolRule(parameters.value(TicketModel::IS_DONE_PARAM)))
            .AddRule(BoolRule(parameters.value(TicketModel::IS_VOICED_PARAM)))
            .AddRule(BoolRule(parameters.value(TicketModel::IS_MANUAL_PARAM)))
            .AddRule(WindowRule(parameters.value(TicketModel::WINDOW_NUMBER_PARAM)))
            .AddRule(IfRule<IntRule, IntRule, AlwaysTrueRule>(
                         checkOnServiceValue,
                         checkIsDoneValue,
                         AlwaysTrueRule()));;
    return validator.Validate();
}

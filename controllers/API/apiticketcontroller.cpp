#include <QIntValidator>
#include "apiticketcontroller.h"
#include "entities/ticket.h"
#include "models/ticketmodel.h"

void ApiTicketController::update(stefanfrings::HttpRequest &request, stefanfrings::HttpResponse &response)
{
    response.setHeader("Content-Type", "application/json");
    if (validateRequest(request) == true) {
        TableOptions options;
        auto params = request.getParameterMap();
        for (auto it = params.cbegin(); it != params.cend(); it++) {
            options.insert(it.key(), it.value());
        }
        Ticket ticket = TicketModel().updateTicket(options);
        if (ticket.isValid()) {
            response.setStatus(200);
        } else {
            setClientError(response);
        }
    } else {
        setClientError(response);
    }
}

bool ApiTicketController::validateRequest(stefanfrings::HttpRequest &request)
{
    bool result = false;
    QByteArray id_param = TicketModel::ID_COL.toUtf8();
    QByteArray is_voiced_param = TicketModel::IS_VOICED_COL.toUtf8();
    int pos = 0;
    auto params = request.getParameterMap();
    if (params.contains(id_param)) {
        QIntValidator id_validator;
        id_validator.setBottom(1);
        QString id(params.value(id_param));
        if (id_validator.validate(id, pos) == QValidator::State::Acceptable) {
            QIntValidator action_validator(0, 1);
            if (params.contains(is_voiced_param)) {
                QString is_voiced(params.value(is_voiced_param));
                if (action_validator.validate(is_voiced, pos) == QValidator::State::Acceptable) {
                    result = true;
                }
            }
        }
    }
    return result;
}

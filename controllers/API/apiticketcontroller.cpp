#include <QIntValidator>
#include "apiticketcontroller.h"
#include "entities/ticket.h"
#include "models/ticketmodel.h"
#include "validators/requestvalidator.h"
#include "validators/rules/idrule.h"
#include "validators/rules/boolrule.h"

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
    auto parameters = request.getParameterMap();
    RequestValidator validator;
    validator.AddRule(IdRule(parameters.value(TicketModel::ID_COL_PARAM)))
            .AddRule(BoolRule(parameters.value(TicketModel::IS_VOICED_PARAM)));
    return validator.Validate();
}

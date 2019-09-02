#include "apiticketcontroller.h"
#include "entities/ticket.h"
#include "models/ticketmodel.h"
#include "validators/rules/boolrule.h"
#include "validators/rules/idrule.h"

void ApiTicketController::update(stefanfrings::HttpRequest &request,
                                 stefanfrings::HttpResponse &response) {
  response.setHeader("Content-Type", "application/json");
  if (validateRequest(request)) {
    TableOptions options;
    auto params = request.getParameterMap();
    for (auto it = params.cbegin(); it != params.cend(); it++) {
      options.insert(it.key(), it.value());
    }
    if (auto ticket = TicketModel().updateTicket(options); ticket) {
      response.setStatus(200);
    } else {
      setClientError(response);
    }
  } else {
    setClientError(response);
  }
}

bool ApiTicketController::validateRequest(stefanfrings::HttpRequest &request) {
  auto parameters = request.getParameterMap();
  return Validate(IdRule(parameters.value(TicketModel::ID_COL_PARAM)),
                  BoolRule(parameters.value(TicketModel::IS_VOICED_PARAM)));
}

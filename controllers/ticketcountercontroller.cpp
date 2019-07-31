#include "ticketcountercontroller.h"
#include "requestmapper.h"
#include "ticketcounter.h"

void TicketCounterController::index(stefanfrings::HttpRequest &,
                                    stefanfrings::HttpResponse &response) {
  response.setHeader("Content-Type", "text/html; charset=UTF-8");
  auto t = RequestMapper::templateCache->getTemplate("counter");
  t.setVariable("count", QString::number(TicketCounter::getInstance().value()));
  response.write(t.toUtf8(), true);
}

void TicketCounterController::update(stefanfrings::HttpRequest &,
                                     stefanfrings::HttpResponse &response) {
  response.setHeader("Content-Type", "text/html; charset=UTF-8");
  auto t = RequestMapper::templateCache->getTemplate("counter");
  auto &ticket_counter = TicketCounter::getInstance();
  ticket_counter.clear();
  t.setVariable("count", "0");
  response.write(t.toUtf8(), true);
}

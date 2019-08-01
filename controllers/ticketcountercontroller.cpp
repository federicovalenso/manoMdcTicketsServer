#include "ticketcountercontroller.h"
#include "requestmapper.h"
#include "ticketcounter.h"

void fillTemplateFromCounter(stefanfrings::Template &t,
                             const TicketCounter &tc) {
  t.setVariable("count", QString::number(tc.value()));
  t.setVariable("last_clear_time", tc.last_clear_time());
  t.setVariable("prev_count", QString::number(tc.prev_value()));
}

void TicketCounterController::index(stefanfrings::HttpRequest &,
                                    stefanfrings::HttpResponse &response) {
  response.setHeader("Content-Type", "text/html; charset=UTF-8");
  auto t = RequestMapper::templateCache->getTemplate("counter");
  auto &tc = TicketCounter::getInstance();
  fillTemplateFromCounter(t, tc);
  response.write(t.toUtf8(), true);
}

void TicketCounterController::update(stefanfrings::HttpRequest &,
                                     stefanfrings::HttpResponse &response) {
  response.setHeader("Content-Type", "text/html; charset=UTF-8");
  auto t = RequestMapper::templateCache->getTemplate("counter");
  auto &tc = TicketCounter::getInstance();
  tc.clear();
  fillTemplateFromCounter(t, tc);
  response.write(t.toUtf8(), true);
}

#ifndef TICKETCOUNTERCONTROLLER_H
#define TICKETCOUNTERCONTROLLER_H

#include "modelcontroller.h"

class TicketCounterController : public ModelController {
 public:
  void index(stefanfrings::HttpRequest &, stefanfrings::HttpResponse &response);
  void update(stefanfrings::HttpRequest &,
              stefanfrings::HttpResponse &response);
};

#endif  // TICKETCOUNTERCONTROLLER_H

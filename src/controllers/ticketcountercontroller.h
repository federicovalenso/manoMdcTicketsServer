#ifndef TICKETCOUNTERCONTROLLER_H
#define TICKETCOUNTERCONTROLLER_H

#include "modelcontroller.h"

class TicketCounterController : public ModelController {
 public:
  void index(stefanfrings::HttpRequest &,
             stefanfrings::HttpResponse &response) const override;
  void update(stefanfrings::HttpRequest &,
              stefanfrings::HttpResponse &response) override;
};

#endif  // TICKETCOUNTERCONTROLLER_H

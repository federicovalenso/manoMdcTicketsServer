#ifndef STATISTICSCONTROLLER_H
#define STATISTICSCONTROLLER_H

#include "modelcontroller.h"

class StatisticsController : public ModelController {
 public:
  StatisticsController() = default;

  void index(stefanfrings::HttpRequest &,
             stefanfrings::HttpResponse &response) const;
  void ticketsCount(stefanfrings::HttpRequest &request,
                    stefanfrings::HttpResponse &response) const;
};

#endif  // STATISTICSCONTROLLER_H

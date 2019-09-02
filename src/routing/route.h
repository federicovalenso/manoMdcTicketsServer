#ifndef ROUTE_H
#define ROUTE_H

#include <QString>
#include <functional>
#include <memory>

#include "controllers/modelcontroller.h"

using Action = std::function<void(std::shared_ptr<ModelController>,
                                  stefanfrings::HttpRequest&,
                                  stefanfrings::HttpResponse&)>;
using ControllerPtr = std::shared_ptr<ModelController>;

class Route {
 public:
  Route(ControllerPtr controller_ptr, Action action);
  void execute(stefanfrings::HttpRequest& request,
               stefanfrings::HttpResponse& response) const;

 private:
  ControllerPtr controller_ptr_;
  Action action_;
};

#endif  // ROUTE_H

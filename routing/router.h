#ifndef ROUTER_H
#define ROUTER_H

#include <QByteArray>
#include <QHash>
#include <QSet>
#include <functional>
#include <memory>
#include <type_traits>

#include "route.h"
#include "routekey.h"

using ControllerName = QString;
using ActionName = QString;

class Router {
 public:
  static Router& getInstance();
  bool executeRoute(stefanfrings::HttpRequest& request,
                    stefanfrings::HttpResponse& response);

 private:
  Router();
  Router(const Router&) = delete;
  Router& operator=(const Router&) = delete;
  const QString USER_CONTROLLER = "UserController";
  const QString TICKET_CONTROLLER = "TicketController";
  const QString API_TICKET_CONTROLLER = "ApiTicketController";
  const QString INDEX_ACTION = "index";
  const QString STORE_ACTION = "store";
  const QString CREATE_ACTION = "create";
  const QString DESTROY_ACTION = "destroy";
  const QString UPDATE_ACTION = "update";
  const QString SHOW_ACTION = "show";
  const QString EDIT_ACTION = "edit";

  QHash<RouteKey, Route> routes_;
  QHash<ControllerName, ControllerFactoryMethod> controllers_;
  QHash<ActionName, Action> actions_;

  template <class T>
  static std::shared_ptr<ModelController> createControllerInstance() {
    static_assert(std::is_base_of<ModelController, T>::value,
                  "ModelController must be base class of controller");
    return std::make_shared<T>();
  }
};

#endif  // ROUTER_H

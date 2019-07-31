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
  const QString kUserController = "UserController";
  const QString kTicketController = "TicketController";
  const QString kApiTicketController = "ApiTicketController";
  const QString kTicketCounterController = "TicketCounterController";
  const QString kIndexAction = "index";
  const QString kStoreAction = "store";
  const QString kCreateAction = "create";
  const QString kDestroyAction = "destroy";
  const QString kUpdateAction = "update";
  const QString kShowAction = "show";
  const QString kEditAction = "edit";

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

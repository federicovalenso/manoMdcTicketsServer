#ifndef ROUTER_H
#define ROUTER_H

#include <QByteArray>
#include <QHash>
#include <QSet>
#include <functional>
#include <memory>
#include "controllers/API/apiticketcontroller.h"
#include "controllers/statisticscontroller.h"
#include "controllers/ticketcontroller.h"
#include "controllers/ticketcountercontroller.h"
#include "controllers/usercontroller.h"
#include "route.h"
#include "routekey.h"

using ControllerName = QString;
using ActionName = QString;

class Router {
 public:
  static Router& getInstance();

  Router(const Router&) = delete;
  Router& operator=(const Router&) = delete;
  Router(Router&&) = delete;
  Router& operator=(Router&&) = delete;

  bool executeRoute(stefanfrings::HttpRequest& request,
                    stefanfrings::HttpResponse& response) const;

 private:
  Router() = default;
  ~Router() = default;

  const ControllerName kUserController = "User";
  const ControllerName kTicketController = "Ticket";
  const ControllerName kApiTicketController = "ApiTicket";
  const ControllerName kTicketCounterController = "TicketCounter";
  const ControllerName kStatisticsController = "Statistics";
  const ActionName kIndex = "index";
  const ActionName kStore = "store";
  const ActionName kCreate = "create";
  const ActionName kDestroy = "destroy";
  const ActionName kUpdate = "update";
  const ActionName kShow = "show";
  const ActionName kEdit = "edit";
  const HttpMethod kGet = "GET";
  const HttpMethod kPost = "POST";
  const HttpMethod kPut = "PUT";
  const Path kLogin = "/login";
  const Path kTickets = "/tickets";
  const Path kFreeTickets = "/tickets/free";
  const Path kApiTickets = "/api/tickets";
  const Path kCounter = "/counter";
  const Path kStatistics = "/statistics";
  const Path kApiStatisticsCount = "/api/statistics/count";

  const QHash<ControllerName, ControllerPtr> kControllers = {
      {kUserController, std::make_shared<UserController>()},
      {kTicketController, std::make_shared<TicketController>()},
      {kApiTicketController, std::make_shared<ApiTicketController>()},
      {kTicketCounterController, std::make_shared<TicketCounterController>()},
      {kStatisticsController, std::make_shared<StatisticsController>()}};
  const QHash<ActionName, Action> kActions{
      {kIndex, std::mem_fn(&ModelController::index)},
      {kStore, std::mem_fn(&ModelController::store)},
      {kCreate, std::mem_fn(&ModelController::create)},
      {kDestroy, std::mem_fn(&ModelController::destroy)},
      {kUpdate, std::mem_fn(&ModelController::update)},
      {kShow, std::mem_fn(&ModelController::show)},
      {kEdit, std::mem_fn(&ModelController::edit)}};

  using BaseControllerFuncPtr = void (ModelController::*)(
      stefanfrings::HttpRequest& request, stefanfrings::HttpResponse& response);
  using BaseControllerConstFuncPtr =
      void (ModelController::*)(stefanfrings::HttpRequest& request,
                                stefanfrings::HttpResponse& response) const;

  const QHash<RouteKey, Route> kRoutes = {
      {RouteKey(kLogin, kPost),
       Route(kControllers.value(kUserController), kActions.value(kShow))},
      {RouteKey(kTickets, kGet),
       Route(kControllers.value(kTicketController), kActions.value(kIndex))},
      {RouteKey(kTickets, kPost),
       Route(kControllers.value(kTicketController), kActions.value(kStore))},
      {RouteKey(kTickets, kPut),
       Route(kControllers.value(kTicketController), kActions.value(kUpdate))},
      {RouteKey(kFreeTickets, kGet),
       Route(kControllers.value(kTicketController), kActions.value(kShow))},
      {RouteKey(kApiTickets, kPut),
       Route(kControllers.value(kApiTicketController),
             kActions.value(kUpdate))},
      {RouteKey(kCounter, kGet),
       Route(kControllers.value(kTicketCounterController),
             kActions.value(kIndex))},
      {RouteKey(kCounter, kPost),
       Route(kControllers.value(kTicketCounterController),
             kActions.value(kUpdate))},
      {RouteKey(kStatistics, kGet),
       Route(kControllers.value(kStatisticsController),
             kActions.value(kIndex))},
      {RouteKey(kApiStatisticsCount, kGet),
       Route(kControllers.value(kStatisticsController),
             static_cast<BaseControllerConstFuncPtr>(
                 &StatisticsController::ticketsCount))}};
};

#endif  // ROUTER_H

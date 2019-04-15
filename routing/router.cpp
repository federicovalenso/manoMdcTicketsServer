#include "router.h"
#include "controllers/API/apiticketcontroller.h"
#include "controllers/ticketcontroller.h"
#include "controllers/usercontroller.h"

#include <QDebug>

Router::Router() {
  using namespace std;
  controllers_.insert(USER_CONTROLLER,
                      Router::createControllerInstance<UserController>);
  controllers_.insert(TICKET_CONTROLLER,
                      Router::createControllerInstance<TicketController>);
  controllers_.insert(API_TICKET_CONTROLLER,
                      Router::createControllerInstance<ApiTicketController>);
  actions_.insert(INDEX_ACTION, mem_fn(&ModelController::index));
  actions_.insert(STORE_ACTION, mem_fn(&ModelController::store));
  actions_.insert(CREATE_ACTION, mem_fn(&ModelController::create));
  actions_.insert(DESTROY_ACTION, mem_fn(&ModelController::destroy));
  actions_.insert(UPDATE_ACTION, mem_fn(&ModelController::update));
  actions_.insert(SHOW_ACTION, mem_fn(&ModelController::show));
  actions_.insert(EDIT_ACTION, mem_fn(&ModelController::edit));
  routes_.insert(
      RouteKey("/login", "POST"),
      Route(controllers_.value(USER_CONTROLLER), actions_.value(SHOW_ACTION)));
  routes_.insert(RouteKey("/tickets", "GET"),
                 Route(controllers_.value(TICKET_CONTROLLER),
                       actions_.value(INDEX_ACTION)));
  routes_.insert(RouteKey("/tickets", "POST"),
                 Route(controllers_.value(TICKET_CONTROLLER),
                       actions_.value(STORE_ACTION)));
  routes_.insert(RouteKey("/tickets", "PUT"),
                 Route(controllers_.value(TICKET_CONTROLLER),
                       actions_.value(UPDATE_ACTION)));
  routes_.insert(RouteKey("/tickets/free", "GET"),
                 Route(controllers_.value(TICKET_CONTROLLER),
                       actions_.value(SHOW_ACTION)));
  routes_.insert(RouteKey("/api/tickets", "PUT"),
                 Route(controllers_.value(API_TICKET_CONTROLLER),
                       actions_.value(UPDATE_ACTION)));
}

bool Router::executeRoute(stefanfrings::HttpRequest &request,
                          stefanfrings::HttpResponse &response) {
  using namespace stefanfrings;
  bool result = false;
  Path path(request.getPath());
  HttpMethod httpMethod(request.getMethod());
  RouteKey routeKey(path, httpMethod);
  auto route = routes_.find(routeKey);
  if (route != routes_.end()) {
    route.value().execute(request, response);
    result = true;
  }
  return result;
}

Router &Router::getInstance() {
  static Router instance;
  return instance;
}

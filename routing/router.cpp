#include "router.h"
#include "controllers/API/apiticketcontroller.h"
#include "controllers/ticketcontroller.h"
#include "controllers/ticketcountercontroller.h"
#include "controllers/usercontroller.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

Router::Router() {
  using namespace std;
  controllers_.insert(kUserController,
                      createControllerInstance<UserController>);
  controllers_.insert(kTicketController,
                      createControllerInstance<TicketController>);
  controllers_.insert(kApiTicketController,
                      createControllerInstance<ApiTicketController>);
  controllers_.insert(kTicketCounterController,
                      createControllerInstance<TicketCounterController>);

  actions_.insert(kIndexAction, mem_fn(&ModelController::index));
  actions_.insert(kStoreAction, mem_fn(&ModelController::store));
  actions_.insert(kCreateAction, mem_fn(&ModelController::create));
  actions_.insert(kDestroyAction, mem_fn(&ModelController::destroy));
  actions_.insert(kUpdateAction, mem_fn(&ModelController::update));
  actions_.insert(kShowAction, mem_fn(&ModelController::show));
  actions_.insert(kEditAction, mem_fn(&ModelController::edit));

  routes_.insert(
      RouteKey("/login", "POST"),
      Route(controllers_.value(kUserController), actions_.value(kShowAction)));
  routes_.insert(RouteKey("/tickets", "GET"),
                 Route(controllers_.value(kTicketController),
                       actions_.value(kIndexAction)));
  routes_.insert(RouteKey("/tickets", "POST"),
                 Route(controllers_.value(kTicketController),
                       actions_.value(kStoreAction)));
  routes_.insert(RouteKey("/tickets", "PUT"),
                 Route(controllers_.value(kTicketController),
                       actions_.value(kUpdateAction)));
  routes_.insert(RouteKey("/tickets/free", "GET"),
                 Route(controllers_.value(kTicketController),
                       actions_.value(kShowAction)));
  routes_.insert(RouteKey("/api/tickets", "PUT"),
                 Route(controllers_.value(kApiTicketController),
                       actions_.value(kUpdateAction)));
  routes_.insert(RouteKey("/counter", "GET"),
                 Route(controllers_.value(kTicketCounterController),
                       actions_.value(kIndexAction)));
  routes_.insert(RouteKey("/counter", "POST"),
                 Route(controllers_.value(kTicketCounterController),
                       actions_.value(kUpdateAction)));
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

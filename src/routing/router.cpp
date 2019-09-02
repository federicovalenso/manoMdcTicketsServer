#include "router.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

bool Router::executeRoute(stefanfrings::HttpRequest &request,
                          stefanfrings::HttpResponse &response) const {
  using namespace stefanfrings;
  bool result = false;
  RouteKey routeKey(request.getPath(), request.getMethod());
  auto route = kRoutes.find(routeKey);
  if (route != kRoutes.end()) {
    route.value().execute(request, response);
    result = true;
  }
  return result;
}

Router &Router::getInstance() {
  static Router instance;
  return instance;
}

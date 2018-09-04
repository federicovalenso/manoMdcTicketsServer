#include "router.h"
#include "controllers/ticketcontroller.h"
#include "controllers/usercontroller.h"

#include <QDebug>

Router::Router()
{
    using namespace std;
    mControllers.insert("TicketController", std::bind(&Router::createInstance<TicketController>, this));
    mControllers.insert("UserController", std::bind(&Router::createInstance<UserController>, this));
    mActions.insert("store", mem_fn(&ModelController::store));
    mActions.insert("index", mem_fn(&ModelController::index));
    mActions.insert("create", mem_fn(&ModelController::create));
    mActions.insert("destroy", mem_fn(&ModelController::destroy));
    mActions.insert("update", mem_fn(&ModelController::update));
    mActions.insert("show", mem_fn(&ModelController::show));
    mActions.insert("edit", mem_fn(&ModelController::edit));
    mRoutes.insert(RouteKey("/login", "POST"), Route(mControllers.value("UserController"), mActions.value("show")));
    mRoutes.insert(RouteKey("/patients", "GET"), Route(mControllers.value("PatientController"), mActions.value("index")));
    mRoutes.insert(RouteKey("/tickets", "GET"), Route(mControllers.value("TicketController"), mActions.value("index")));
    mRoutes.insert(RouteKey("/tickets", "POST"), Route(mControllers.value("TicketController"), mActions.value("store")));
    mRoutes.insert(RouteKey("/tickets", "PUT"), Route(mControllers.value("TicketController"), mActions.value("update")));
    mRoutes.insert(RouteKey("/tickets/free", "GET"), Route(mControllers.value("TicketController"), mActions.value("show")));
}

bool Router::executeRoute(stefanfrings::HttpRequest & request, stefanfrings::HttpResponse & response)
{
    using namespace stefanfrings;
    bool result = false;
    Path path = Path(request.getPath());
    HttpMethod httpMethod = HttpMethod(request.getMethod());
    RouteKey routeKey(path, httpMethod);
    auto route = mRoutes.find(routeKey);
    if (route != mRoutes.end()) {
        route.value().execute(request, response);
        result = true;
    }
    return result;
}

Router &Router::getInstance()
{
    static Router instance;
    return instance;
}

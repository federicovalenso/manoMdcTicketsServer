#include "router.h"
#include "controllers/ticketcontroller.h"
#include "controllers/usercontroller.h"
#include "controllers/API/apiticketcontroller.h"

#include <QDebug>

Router::Router()
{
    using namespace std;
    mControllers.insert(USER_CONTROLLER, std::bind(&Router::createInstance<UserController>, this));
    mControllers.insert(TICKET_CONTROLLER, std::bind(&Router::createInstance<TicketController>, this));
    mControllers.insert(API_TICKET_CONTROLLER, std::bind(&Router::createInstance<ApiTicketController>, this));
    mActions.insert(INDEX_ACTION, mem_fn(&ModelController::index));
    mActions.insert(STORE_ACTION, mem_fn(&ModelController::store));
    mActions.insert(CREATE_ACTION, mem_fn(&ModelController::create));
    mActions.insert(DESTROY_ACTION, mem_fn(&ModelController::destroy));
    mActions.insert(UPDATE_ACTION, mem_fn(&ModelController::update));
    mActions.insert(SHOW_ACTION, mem_fn(&ModelController::show));
    mActions.insert(EDIT_ACTION, mem_fn(&ModelController::edit));
    mRoutes.insert(RouteKey("/login", "POST"), Route(mControllers.value(USER_CONTROLLER), mActions.value(SHOW_ACTION)));
    mRoutes.insert(RouteKey("/tickets", "GET"), Route(mControllers.value(TICKET_CONTROLLER), mActions.value(INDEX_ACTION)));
    mRoutes.insert(RouteKey("/tickets", "POST"), Route(mControllers.value(TICKET_CONTROLLER), mActions.value(STORE_ACTION)));
    mRoutes.insert(RouteKey("/tickets", "PUT"), Route(mControllers.value(TICKET_CONTROLLER), mActions.value(UPDATE_ACTION)));
    mRoutes.insert(RouteKey("/tickets/free", "GET"), Route(mControllers.value(TICKET_CONTROLLER), mActions.value(SHOW_ACTION)));
    mRoutes.insert(RouteKey("/api/tickets", "PUT"), Route(mControllers.value(API_TICKET_CONTROLLER), mActions.value(UPDATE_ACTION)));
}

bool Router::executeRoute(stefanfrings::HttpRequest & request, stefanfrings::HttpResponse & response)
{
    using namespace stefanfrings;
    bool result = false;
    Path path(request.getPath());
    HttpMethod httpMethod(request.getMethod());
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

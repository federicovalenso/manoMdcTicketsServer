#ifndef ROUTER_H
#define ROUTER_H

#include <QSet>
#include <QByteArray>
#include <QHash>
#include <functional>
#include <memory>

#include "route.h"
#include "routekey.h"

using ControllerName = QString;
using ActionName = QString;

class Router
{
public:
    static Router& getInstance();
    bool executeRoute(stefanfrings::HttpRequest & request, stefanfrings::HttpResponse & response);

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

    QHash<RouteKey, Route> mRoutes;

    template <class T>
    std::shared_ptr<ModelController> createInstance()
    {
        return std::make_shared<T>();
    }
    QHash<ControllerName, ControllerFactoryMethod> mControllers;
    QHash<ActionName, Action> mActions;
};

#endif // ROUTER_H

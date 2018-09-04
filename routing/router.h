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

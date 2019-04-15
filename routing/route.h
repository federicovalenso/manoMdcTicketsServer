#ifndef ROUTE_H
#define ROUTE_H

#include <QString>
#include <functional>
#include <memory>

#include "controllers/modelcontroller.h"

using Action = std::function<void(std::shared_ptr<ModelController>,
                                  stefanfrings::HttpRequest&,
                                  stefanfrings::HttpResponse&)>;
using ControllerFactoryMethod =
    std::function<std::shared_ptr<ModelController>(void)>;

class Route {
 public:
  Route(ControllerFactoryMethod factoryMethod, Action action);
  void execute(stefanfrings::HttpRequest& request,
               stefanfrings::HttpResponse& response);

 private:
  /*!
   * @brief mFactoryMethod
   * Фабричный метод, который создаст контроллер
   */
  ControllerFactoryMethod mFactoryMethod;
  /**
   * @brief mAction
   */
  Action mAction;
};

#endif  // ROUTE_H

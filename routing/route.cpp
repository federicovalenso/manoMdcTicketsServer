#include "route.h"

Route::Route(ControllerFactoryMethod factoryMethod,
             Action action) :
    mFactoryMethod(factoryMethod),
    mAction(action)
{}

void Route::execute(stefanfrings::HttpRequest & request, stefanfrings::HttpResponse & response)
{
    mAction(mFactoryMethod(), request, response);
}

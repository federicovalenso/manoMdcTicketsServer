/**
  @file
  @author Stefan Frings
*/

#ifndef REQUESTMAPPER_H
#define REQUESTMAPPER_H

#include <QString>

#include "httprequesthandler.h"
#include "controllers/modelcontroller.h"

using namespace stefanfrings;

class RequestMapper : public HttpRequestHandler {
    Q_OBJECT
    Q_DISABLE_COPY(RequestMapper)
public:

    RequestMapper(QObject* parent=0);
    ~RequestMapper();

    /**
      Передает входящий HTTP-запрос в роутер.
      Если роутер не смог обработать запрос, передаёт запрос в статик-контроллер
      @param request Http-запрос
      @param response Используется для вывода ответа на запрос
    */
    void service(HttpRequest& request, HttpResponse& response);
};

#endif // REQUESTMAPPER_H

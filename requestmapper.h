/**
  @file
  @author Stefan Frings
*/

#ifndef REQUESTMAPPER_H
#define REQUESTMAPPER_H

#include <QString>

#include "controllers/modelcontroller.h"
#include "httprequesthandler.h"
#include "logger.h"
#include "staticfilecontroller.h"
#include "templatecache.h"

using namespace stefanfrings;

class RequestMapper : public HttpRequestHandler {
  Q_OBJECT
  Q_DISABLE_COPY(RequestMapper)
 public:
  static HttpSessionStore* sessionStore;
  static StaticFileController* staticFileController;
  static TemplateCache* templateCache;
  static Logger* logger;

  RequestMapper(QObject* parent = nullptr);
  ~RequestMapper();

  /**
    Передает входящий HTTP-запрос в роутер.
    Если роутер не смог обработать запрос, передаёт запрос в статик-контроллер
    @param request Http-запрос
    @param response Используется для вывода ответа на запрос
  */
  void service(HttpRequest& request, HttpResponse& response);
};

#endif  // REQUESTMAPPER_H

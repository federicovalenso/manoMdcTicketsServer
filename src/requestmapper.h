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

class RequestMapper : public stefanfrings::HttpRequestHandler {
  Q_OBJECT
  Q_DISABLE_COPY(RequestMapper)
 public:
  static stefanfrings::HttpSessionStore* sessionStore;
  static stefanfrings::StaticFileController* staticFileController;
  static stefanfrings::TemplateCache* templateCache;
  static stefanfrings::Logger* logger;

  RequestMapper(QObject* parent = nullptr);
  ~RequestMapper() override = default;

  /**
    Передает входящий HTTP-запрос в роутер.
    Если роутер не смог обработать запрос, передаёт запрос в статик-контроллер
    @param request Http-запрос
    @param response Используется для вывода ответа на запрос
  */
  void service(stefanfrings::HttpRequest& request,
               stefanfrings::HttpResponse& response) override;
};

#endif  // REQUESTMAPPER_H

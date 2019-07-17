/**
  @file
  @author Stefan Frings
*/

#include "requestmapper.h"
#include <QCoreApplication>
#include "routing/router.h"

HttpSessionStore* RequestMapper::sessionStore = nullptr;
StaticFileController* RequestMapper::staticFileController = nullptr;
TemplateCache* RequestMapper::templateCache = nullptr;
Logger* RequestMapper::logger = nullptr;

RequestMapper::RequestMapper(QObject* parent) : HttpRequestHandler(parent) {}

void RequestMapper::service(HttpRequest& request, HttpResponse& response) {
  QByteArray path = request.getPath();
  qDebug("RequestMapper: path=%s", path.data());
  Router& router = Router::getInstance();

  if (!router.executeRoute(request, response)) {
    staticFileController->service(request, response);
  }
  qDebug("RequestMapper: finished request");

  // Clear the log buffer
  if (logger) {
    logger->clear();
  }
}

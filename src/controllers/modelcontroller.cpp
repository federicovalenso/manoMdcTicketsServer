#include "modelcontroller.h"

void ModelController::store(stefanfrings::HttpRequest &,
                            stefanfrings::HttpResponse &) {}

void ModelController::index(stefanfrings::HttpRequest &,
                            stefanfrings::HttpResponse &) const {}

void ModelController::create(stefanfrings::HttpRequest &,
                             stefanfrings::HttpResponse &) {}

void ModelController::destroy(stefanfrings::HttpRequest &,
                              stefanfrings::HttpResponse &) {}

void ModelController::update(stefanfrings::HttpRequest &,
                             stefanfrings::HttpResponse &) {}

void ModelController::show(stefanfrings::HttpRequest &,
                           stefanfrings::HttpResponse &) const {}

void ModelController::edit(stefanfrings::HttpRequest &,
                           stefanfrings::HttpResponse &) {}

void ModelController::setClientError(
    stefanfrings::HttpResponse &response) noexcept {
  response.setHeader("Bad Request", 400);
  response.setStatus(400, "Error");
}

void ModelController::setServerError(
    stefanfrings::HttpResponse &response) noexcept {
  response.setHeader("Internal Server Error", 500);
  response.setStatus(500, "Error");
}

void ModelController::setUnauthorizedError(
    stefanfrings::HttpResponse &response) noexcept {
  response.setHeader("Authorization's required", 403);
  response.setStatus(403, "Error");
}

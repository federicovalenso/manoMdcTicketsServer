#include "modelcontroller.h"

ModelController::ModelController()
{}

void ModelController::store(stefanfrings::HttpRequest &, stefanfrings::HttpResponse &)
{
    qDebug() << "store";
}

void ModelController::index(stefanfrings::HttpRequest &, stefanfrings::HttpResponse &)
{
    qDebug() << "Modelcontroller => index";
}

void ModelController::create(stefanfrings::HttpRequest &, stefanfrings::HttpResponse &)
{
    qDebug() << "create";
}

void ModelController::destroy(stefanfrings::HttpRequest &, stefanfrings::HttpResponse &)
{
    qDebug() << "destroy";
}

void ModelController::update(stefanfrings::HttpRequest &, stefanfrings::HttpResponse &)
{
    qDebug() << "update";
}

void ModelController::show(stefanfrings::HttpRequest &, stefanfrings::HttpResponse &)
{
    qDebug() << "show";
}

void ModelController::edit(stefanfrings::HttpRequest &, stefanfrings::HttpResponse &)
{
    qDebug() << "edit";
}

void ModelController::setClientError(stefanfrings::HttpResponse &response) noexcept
{
    response.setHeader("Bad Request", 400);
    response.setStatus(500, "Error");
}

void ModelController::setServerError(stefanfrings::HttpResponse &response) noexcept
{
    response.setHeader("Internal Server Error", 500);
    response.setStatus(500, "Error");
}

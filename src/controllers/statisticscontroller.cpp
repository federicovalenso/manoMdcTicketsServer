#include "statisticscontroller.h"
#include "modelcontroller.h"
#include "models/statisticsmodel.h"
#include "requestmapper.h"

void StatisticsController::index(stefanfrings::HttpRequest &,
                                 stefanfrings::HttpResponse &response) const {
  response.setHeader("Content-Type", "text/html; charset=UTF-8");
  auto t = RequestMapper::templateCache->getTemplate("statistics");
  response.write(t.toUtf8(), true);
}

void StatisticsController::ticketsCount(
    stefanfrings::HttpRequest &request,
    stefanfrings::HttpResponse &response) const {
  auto byteArrayToDate = [](QByteArray &&str) -> QDate {
    auto date = QDate::fromString(str, "yyyy-MM-dd");
    return date.isValid() ? date : QDate::currentDate();
  };
  auto from = byteArrayToDate(request.getParameter("from"));
  auto to = byteArrayToDate(request.getParameter("to"));
  if (from > to) {
    ModelController::setClientError(response);
    return;
  }
  response.write(StatisticsModel().getCountsByInterval(from, to).toJson(
      QJsonDocument::JsonFormat::Compact));
}

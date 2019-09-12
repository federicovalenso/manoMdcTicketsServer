#include "statisticscontroller.h"
#include <QJsonArray>
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
  auto counts_by_hour = StatisticsModel().getCountsByInterval(from, to);
  QJsonArray labels;
  QJsonArray data;
  for (const auto &count_by_hour : counts_by_hour) {
    labels.push_back(count_by_hour.created_hour);
    data.push_back(count_by_hour.count);
  }
  QJsonArray datasets;
  datasets.push_back(QJsonObject{{"data", data},
                                 {"label", QObject::tr("Количество талонов")},
                                 {"backgroundColor", "#f87979"}});
  QJsonDocument doc{QJsonObject{{"labels", labels}, {"datasets", datasets}}};
  response.write(doc.toJson(QJsonDocument::JsonFormat::Compact));
}

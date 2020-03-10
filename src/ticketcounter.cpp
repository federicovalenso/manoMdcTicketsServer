#include "ticketcounter.h"
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <memory>
#include "serversettings.h"

const QByteArray TicketCounter::kClassName = "TicketCounter";

TicketCounter &TicketCounter::instance() {
  return *qApp->findChild<TicketCounter *>();
}

TicketCounter::TicketCounter(QCoreApplication *parent, QSettings *settings)
    : QObject(parent), kNotificator([settings] {
        return Notificator(
            settings->value(Settings::kCriticalCount).toUInt(),
            settings->value(Settings::kNotificationService).toString(),
            settings->value(Settings::kMailList).toStringList());
      }()) {
  counter_ = settings->value(Settings::kCurrentCount).toUInt();
  prev_count_ = settings->value(Settings::kPrevCount).toUInt();
  last_clear_time_ = settings->value(Settings::kLastClearTime).toDateTime();
}

TicketCounter::~TicketCounter() {
  auto settings_file = parent()->property(Settings::kSettingsFile).toString();
  QSettings counter_settings(settings_file, QSettings::IniFormat);
  counter_settings.beginGroup(Settings::kCounterGroup);
  counter_settings.setValue(Settings::kCurrentCount, counter_.load());
  counter_settings.setValue(Settings::kPrevCount, prev_count_);
  counter_settings.setValue(Settings::kLastClearTime, last_clear_time_);
}

TicketCounter &TicketCounter::operator++() {
  ++counter_;
  if (counter_ == kNotificator.CriticalCount()) kNotificator.send();
  return *this;
}

void TicketCounter::clear() {
  prev_count_ = counter_;
  counter_ = 0;
  last_clear_time_ = QDateTime::currentDateTime();
}

uint32_t TicketCounter::Notificator::CriticalCount() const {
  return kCriticalCount;
}

void TicketCounter::Notificator::sendMail(QString &&mail) const {
  auto network_manager = std::make_unique<QNetworkAccessManager>(nullptr);
  bool success = true;
  QEventLoop loop;
  QObject::connect(network_manager.get(), &QNetworkAccessManager::finished,
                   [&loop, &success] {
                     if (success) {
                       qInfo("Counter: notification's been sended");
                     }
                     loop.exit();
                   });
  QNetworkRequest request(kService);
  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
  QJsonObject data{
      {"To", mail},
      {"Subject", "Сервис очереди в регистратуру"},
      {"Body", QString("Значение счётчика талонов: %1").arg(kCriticalCount)}};
  QJsonDocument doc(data);
  auto reply = network_manager->post(
      request, doc.toJson(QJsonDocument::JsonFormat::Compact));
  QObject::connect(
      reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
      [&reply, &loop, &success] {
        qWarning() << reply->errorString();
        qWarning("Counter: error while sending notification");
        success = false;
        loop.exit();
      });
  loop.exec();
}

TicketCounter::Notificator::Notificator(const uint32_t critical_count,
                                        QString &&service,
                                        QStringList &&mail_list)
    : kCriticalCount(critical_count), kService(service), kMailList(mail_list) {}

void TicketCounter::Notificator::send() const {
  QString mail;
  foreach (mail, kMailList) { sendMail(std::move(mail)); }
}

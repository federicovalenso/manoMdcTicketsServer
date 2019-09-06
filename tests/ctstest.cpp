#include "ctstest.h"
#include <QCoreApplication>
#include <QEventLoop>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMutex>
#include <QMutexLocker>
#include <QNetworkCookieJar>
#include <QSettings>
#include <QStringList>
#include <QUrlQuery>
#include <atomic>
#include <iostream>
#include <stdexcept>
#include "databasemigrator.h"
#include "filelogger.h"
#include "httplistener.h"
#include "httpsessionstore.h"
#include "requestmapper.h"
#include "serversettings.h"
#include "staticfilecontroller.h"
#include "templatecache.h"
#include "ticketcounter.h"

using namespace stefanfrings;

const QString kId = "id";
const QString kTicketAction = "action_name";
const QString kTicketNumber = "ticket_number";
const QString kCreatedAt = "created_at";
const QString kWindow = "window";
const QString kOnService = "on_service";
const QString kIsDone = "is_done";
const QString kIsVoiced = "is_voiced";
const QString kIsManual = "is_manual";

CtsTest::CtsTest(QObject* parent) : QObject(parent) {
  network_access_manager_ = new QNetworkAccessManager(qApp);
  Settings::InitSettings();

  try {
    DatabaseMigrator dbm;
    dbm.migrate();
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    std::exit(EXIT_FAILURE);
  }

  auto app = qApp;
  auto settings_file = app->property(Settings::kSettingsFile).toString();

  QSettings* templateSettings =
      new QSettings(settings_file, QSettings::IniFormat, app);
  templateSettings->beginGroup("templates");
  RequestMapper::templateCache = new TemplateCache(templateSettings, app);

  QSettings* sessionSettings =
      new QSettings(settings_file, QSettings::IniFormat, app);
  sessionSettings->beginGroup("sessions");
  RequestMapper::sessionStore = new HttpSessionStore(sessionSettings, app);

  QSettings* fileSettings =
      new QSettings(settings_file, QSettings::IniFormat, app);
  fileSettings->beginGroup("docroot");
  RequestMapper::staticFileController =
      new StaticFileController(fileSettings, app);

  QSettings* listenerSettings =
      new QSettings(settings_file, QSettings::IniFormat, app);
  listenerSettings->beginGroup("listener");
  new HttpListener(listenerSettings, new RequestMapper(app), app);
  new TicketCounter(app);
  port_ = listenerSettings->value("port").toInt();

  qAddPostRoutine([]() {
    auto connection_names = QSqlDatabase::connectionNames();
    for (const auto& connection_name : connection_names) {
      qDebug() << "Removing connection: " << connection_name;
      QSqlDatabase::removeDatabase(connection_name);
    }
  });
}

void CtsTest::init() { QCoreApplication::processEvents(); }

QNetworkCookie getCookie(const QNetworkReply& reply) noexcept {
  QNetworkCookie result;
  const QByteArray kCookieHeader = "Set-Cookie";
  if (reply.hasRawHeader(kCookieHeader)) {
    auto cookie_header = reply.rawHeader(kCookieHeader);
    auto cookie_parts = cookie_header.split(';');
    for (const auto& part : cookie_parts) {
      auto params = part.split('=');
      if (params.size() == 2) {
        if (params.first() == "sessionid") {
          result.setName(params.first());
          result.setValue(params.last());
        }
      }
    }
  }
  return result;
}

void CtsTest::TestAuthorization() {
  QString address = QString("http://%1:%2/login").arg(kLocalHost).arg(port_);
  QUrl url(address);
  QNetworkRequest request(url);
  request.setHeader(QNetworkRequest::ContentTypeHeader,
                    "application/x-www-form-urlencoded");
  QUrlQuery params;
  params.addQueryItem("name", "Администратор");
  params.addQueryItem("password", "D@kthf90");
  network_access_manager_->post(request,
                                params.toString(QUrl::FullyEncoded).toUtf8());
  QEventLoop loop;
  connect(
      network_access_manager_, &QNetworkAccessManager::finished,
      [&](QNetworkReply* reply) {
        loop.quit();
        QCOMPARE(reply->error(), QNetworkReply::NetworkError::NoError);
        network_access_manager_->cookieJar()->insertCookie(getCookie(*reply));
      });
  QCoreApplication::processEvents();
  loop.exec();
  disconnect(network_access_manager_, &QNetworkAccessManager::finished, nullptr,
             nullptr);
}

void CtsTest::TestSimplePost() {
  QString address = QString("http://%1:%2/tickets").arg(kLocalHost).arg(port_);
  QUrl url(address);
  QNetworkRequest request(url);
  request.setHeader(QNetworkRequest::ContentTypeHeader,
                    "application/x-www-form-urlencoded");
  {
    QUrlQuery params;
    params.addQueryItem("action", "book");
    network_access_manager_->post(request,
                                  params.toString(QUrl::FullyEncoded).toUtf8());
    QEventLoop loop;
    connect(network_access_manager_, &QNetworkAccessManager::finished,
            [&](QNetworkReply* reply) {
              loop.quit();
              QCOMPARE(reply->error(), QNetworkReply::NetworkError::NoError);
            });
    QCoreApplication::processEvents();
    loop.exec();
    disconnect(network_access_manager_, &QNetworkAccessManager::finished,
               nullptr, nullptr);
  }
  {
    QUrlQuery params;
    params.addQueryItem("action", "dummy_action");
    network_access_manager_->post(request,
                                  params.toString(QUrl::FullyEncoded).toUtf8());
    QEventLoop loop;
    connect(network_access_manager_, &QNetworkAccessManager::finished,
            [&](QNetworkReply* reply) {
              loop.quit();
              QVERIFY(reply->error() != QNetworkReply::NetworkError::NoError);
            });
    QCoreApplication::processEvents();
    loop.exec();
    disconnect(network_access_manager_, &QNetworkAccessManager::finished,
               nullptr, nullptr);
  }
}

struct Ticket {
  QString ticket_number;
  QString action;
  int id = -1;
  bool on_service = false;
  bool is_done = false;
  bool is_voiced = false;
  bool is_manual = false;
  qint64 created_at = 0;

  inline bool isValid() const { return id != -1; }
  inline void makeInvalid() { id = -1; }
};

inline bool operator<(const Ticket& lhs, const Ticket& rhs) {
  return lhs.created_at < rhs.created_at;
}

bool isValidTicket(const QJsonObject& ticket) {
  return ticket.contains("id") && ticket.contains("action_name") &&
         ticket.contains("created_at");
}

Ticket ExtractTicketFromJson(QJsonObject object) {
  Ticket result;
  result.id = object.value(kId).toInt();
  result.action = object.value(kTicketAction).toString();
  result.on_service = object.value(kOnService).toBool();
  result.is_done = object.value(kIsDone).toBool();
  result.is_voiced = object.value(kIsVoiced).toBool();
  result.is_manual = object.value(kIsManual).toBool();
  result.created_at = object.value(kCreatedAt).toInt();
  return result;
}

void CtsTest::TestParallelAndIncorrectUpdate() {
  QString address = QString("http://%1:%2/tickets?%3=0")
                        .arg(kLocalHost)
                        .arg(port_)
                        .arg(kOnService);
  QUrl url(address);
  QNetworkRequest request(url);
  QJsonArray json_tickets;
  {
    network_access_manager_->get(request);
    QEventLoop loop;
    connect(network_access_manager_, &QNetworkAccessManager::finished,
            [&json_tickets, &loop](QNetworkReply* reply) {
              loop.quit();
              QVERIFY(reply->error() == QNetworkReply::NetworkError::NoError);
              const QJsonDocument jsonInput =
                  QJsonDocument::fromJson(reply->readAll());
              QVERIFY(!jsonInput.isEmpty());
              QVERIFY(jsonInput.isArray());
              json_tickets = jsonInput.array();
            });
    QCoreApplication::processEvents();
    loop.exec();
    disconnect(network_access_manager_, &QNetworkAccessManager::finished,
               nullptr, nullptr);
  }
  QVERIFY(json_tickets.size() > 0);
  {
    auto ticket = ExtractTicketFromJson(json_tickets[0].toObject());
    ticket.on_service = true;
    auto fromBool = [](bool value) { return value ? "1" : "0"; };
    QUrlQuery params;
    params.addQueryItem(kId, QString::number(ticket.id));
    params.addQueryItem(kOnService, fromBool(ticket.on_service));
    params.addQueryItem(kIsDone, fromBool(ticket.is_done));
    params.addQueryItem(kIsVoiced, fromBool(ticket.is_voiced));
    params.addQueryItem(kIsManual, fromBool(ticket.is_manual));
    params.addQueryItem(kWindow, "1");

    QString address =
        QString("http://%1:%2/tickets").arg(kLocalHost).arg(port_);
    QUrl url(address);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      "application/x-www-form-urlencoded");
    {
      network_access_manager_->put(
          request, params.toString(QUrl::FullyEncoded).toUtf8());
      network_access_manager_->put(
          request, params.toString(QUrl::FullyEncoded).toUtf8());
      network_access_manager_->put(
          request, params.toString(QUrl::FullyEncoded).toUtf8());
      QEventLoop loop;
      struct {
        uint32_t success = 0;
        uint32_t conflict = 0;
      } counter;
      QMutex mutex;
      connect(network_access_manager_, &QNetworkAccessManager::finished,
              [&](QNetworkReply* reply) {
                QMutexLocker locker(&mutex);
                if (reply->error() == QNetworkReply::NetworkError::NoError) {
                  ++counter.success;
                } else {
                  ++counter.conflict;
                }
                if (counter.success + counter.conflict == 3) loop.quit();
              });
      QCoreApplication::processEvents();
      loop.exec();
      disconnect(network_access_manager_, &QNetworkAccessManager::finished,
                 nullptr, nullptr);
      QCOMPARE(counter.success, 1);
      QCOMPARE(counter.conflict, 2);
    }
    {
      network_access_manager_->put(
          request, params.toString(QUrl::FullyEncoded).toUtf8());
      QEventLoop loop;
      connect(network_access_manager_, &QNetworkAccessManager::finished,
              [&](QNetworkReply* reply) {
                loop.quit();
                QVERIFY(reply->error() != QNetworkReply::NetworkError::NoError);
              });
      QCoreApplication::processEvents();
      loop.exec();
      disconnect(network_access_manager_, &QNetworkAccessManager::finished,
                 nullptr, nullptr);
    }
  }
}

void CtsTest::TestStatistics() {
  QString address =
      QString("http://%1:%2/api/statistics/count").arg(kLocalHost).arg(port_);
  QUrl url(address);
  QNetworkRequest request(url);
  {
    network_access_manager_->get(request);
    QEventLoop loop;
    connect(network_access_manager_, &QNetworkAccessManager::finished,
            [&loop](QNetworkReply* reply) {
              loop.quit();
              QVERIFY(reply->error() == QNetworkReply::NetworkError::NoError);
            });
    QCoreApplication::processEvents();
    loop.exec();
    disconnect(network_access_manager_, &QNetworkAccessManager::finished,
               nullptr, nullptr);
  }
}

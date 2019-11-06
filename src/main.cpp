#if defined(unix) || defined(__unix__) || defined(__unix)
#define PREDEF_PLATFORM_UNIX
#endif

#include <QCoreApplication>
#include <QStringList>

#include "databasemigrator.h"
#include "filelogger.h"
#include "httplistener.h"
#include "httpsessionstore.h"
#include "requestmapper.h"
#include "serversettings.h"
#include "staticfilecontroller.h"
#include "templatecache.h"
#include "ticketcounter.h"

#include <iostream>
#include <stdexcept>

#ifdef PREDEF_PLATFORM_UNIX
#include <signal.h>
#include <unistd.h>
#include <initializer_list>

void catchUnixSignals(std::initializer_list<int> quitSignals) {
  auto handler = [](int sig) -> void {
    qWarning("Application is closing by system signal...");
    QCoreApplication::quit();
  };

  sigset_t blocking_mask;
  sigemptyset(&blocking_mask);
  for (auto sig : quitSignals) sigaddset(&blocking_mask, sig);

  struct sigaction sa;
  sa.sa_handler = handler;
  sa.sa_mask = blocking_mask;
  sa.sa_flags = 0;

  for (auto sig : quitSignals) sigaction(sig, &sa, nullptr);
}
#endif

using namespace stefanfrings;

int main(int argc, char* argv[]) {
  QStringList paths = QCoreApplication::libraryPaths();
  paths.append(".");
  paths.append("platforms");
  paths.append("sqldrivers");
  QCoreApplication::setLibraryPaths(paths);
  QCoreApplication app(argc, argv);
  app.setOrganizationName("MANO \'MDC\'");
  app.setApplicationName("Mano \'MDC\' Server App");
#ifdef PREDEF_PLATFORM_UNIX
  catchUnixSignals({SIGQUIT, SIGINT, SIGTERM, SIGHUP});
#endif
  Settings::InitSettings();

  try {
    DatabaseMigrator dbm;
    dbm.migrate();
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    std::exit(EXIT_FAILURE);
  }

  auto settings_file = app.property(Settings::kSettingsFile).toString();

#ifndef QT_DEBUG
  QSettings* logSettings =
      new QSettings(settings_file, QSettings::IniFormat, &app);
  logSettings->beginGroup("logging");
  RequestMapper::logger = new FileLogger(logSettings, 10000, &app);
  RequestMapper::logger->installMsgHandler();
#endif

  QSettings* templateSettings =
      new QSettings(settings_file, QSettings::IniFormat, &app);
  templateSettings->beginGroup("templates");
  RequestMapper::templateCache = new TemplateCache(templateSettings, &app);

  QSettings* sessionSettings =
      new QSettings(settings_file, QSettings::IniFormat, &app);
  sessionSettings->beginGroup("sessions");
  RequestMapper::sessionStore = new HttpSessionStore(sessionSettings, &app);

  QSettings* fileSettings =
      new QSettings(settings_file, QSettings::IniFormat, &app);
  fileSettings->beginGroup("docroot");
  RequestMapper::staticFileController =
      new StaticFileController(fileSettings, &app);

  QSettings* listenerSettings =
      new QSettings(settings_file, QSettings::IniFormat, &app);
  listenerSettings->beginGroup("listener");
  new HttpListener(listenerSettings, new RequestMapper(&app), &app);
  new TicketCounter(&app);

  qWarning("Application has started");
  app.exec();

  qWarning("Application has stopped");
  return 0;
}

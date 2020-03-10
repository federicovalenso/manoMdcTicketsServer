#if defined(unix) || defined(__unix__) || defined(__unix)
#define PREDEF_PLATFORM_UNIX
#endif

#include <QCoreApplication>
#include <QStringList>

#include "databasemigrator.h"
#include "httplistener.h"
#include "httpsessionstore.h"
#include "requestmapper.h"
#include "serversettings.h"
#include "staticfilecontroller.h"
#include "templatecache.h"
#include "ticketcounter.h"
#ifdef FILELOGGING
#include "filelogger.h"
#endif

#include <iostream>
#include <stdexcept>

#ifdef WIN32
#include <stdio.h>
#include <windows.h>

BOOL WINAPI CtrlHandler(DWORD fdwCtrlType) {
  BOOL result;
  switch (fdwCtrlType) {
    case CTRL_C_EVENT:
      qWarning("Application is closing by Ctrl-C event...");
      result = TRUE;
      break;

    case CTRL_CLOSE_EVENT:
      qWarning("Application is closing by Ctrl-Close event...");
      result = TRUE;
      break;

    case CTRL_BREAK_EVENT:
      qWarning("Application is closing by Ctrl-Break event...");
      result = FALSE;
      break;

    case CTRL_LOGOFF_EVENT:
      qWarning("Application is closing by Ctrl-Logoff event...");
      result = FALSE;
      break;

    case CTRL_SHUTDOWN_EVENT:
      qWarning("Application is closing by Ctrl-Shutdown event...");
      result = FALSE;
      break;

    default:
      qWarning("Application is closing by unknown event...");
      result = FALSE;
      break;
  }
  QCoreApplication::quit();
  return result;
}
#endif

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
#ifdef WIN32
  if (SetConsoleCtrlHandler(CtrlHandler, TRUE)) {
    qInfo("The Control Handler for Windows OS is installed.");
  } else {
    qWarning("Could not set control handler for Windows OS");
    return 1;
  }
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

#ifdef FILELOGGING
  QSettings* logSettings =
      new QSettings(settings_file, QSettings::IniFormat, &app);
  logSettings->beginGroup("logging");
  RequestMapper::logger = new FileLogger(logSettings, 10000, &app);
  RequestMapper::logger->installMsgHandler();
#endif

  auto templateSettings =
      new QSettings(settings_file, QSettings::IniFormat, &app);
  templateSettings->beginGroup("templates");
  RequestMapper::templateCache = new TemplateCache(templateSettings, &app);

  auto sessionSettings =
      new QSettings(settings_file, QSettings::IniFormat, &app);
  sessionSettings->beginGroup("sessions");
  RequestMapper::sessionStore = new HttpSessionStore(sessionSettings, &app);

  auto fileSettings = new QSettings(settings_file, QSettings::IniFormat, &app);
  fileSettings->beginGroup("docroot");
  RequestMapper::staticFileController =
      new StaticFileController(fileSettings, &app);

  auto listenerSettings =
      new QSettings(settings_file, QSettings::IniFormat, &app);
  listenerSettings->beginGroup("listener");
  new HttpListener(listenerSettings, new RequestMapper(&app), &app);

  auto counter_settings =
      new QSettings(settings_file, QSettings::IniFormat, &app);
  counter_settings->beginGroup(Settings::kCounterGroup);
  new TicketCounter(&app, counter_settings);

  qWarning("Application has started");
  app.exec();

  qWarning("Application's been stopped");
  return 0;
}

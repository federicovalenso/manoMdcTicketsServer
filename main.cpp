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

#include <iostream>
#include <stdexcept>

using namespace stefanfrings;

static HttpListener* listener;

int main(int argc, char* argv[]) {
  QStringList paths = QCoreApplication::libraryPaths();
  paths.append(".");
  paths.append("platforms");
  paths.append("sqldrivers");
  paths.append("imageformats");
  paths.append("iconengines");
  QCoreApplication::setLibraryPaths(paths);
  QCoreApplication app(argc, argv);
  app.setOrganizationName("MANO \'MDC\'");
  app.setApplicationName("Mano \'MDC\' Server App");

  QString configFileName = ServerSettings::getInstance().getSettingsFile();

  try {
    DatabaseMigrator dbm;
    dbm.migrate();
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    std::exit(EXIT_FAILURE);
  }

  QSettings* logSettings =
      new QSettings(configFileName, QSettings::IniFormat, &app);
  logSettings->beginGroup("logging");
  RequestMapper::logger = new FileLogger(logSettings, 10000, &app);
  RequestMapper::logger->installMsgHandler();

  QSettings* templateSettings =
      new QSettings(configFileName, QSettings::IniFormat, &app);
  templateSettings->beginGroup("templates");
  RequestMapper::templateCache = new TemplateCache(templateSettings, &app);

  QSettings* sessionSettings =
      new QSettings(configFileName, QSettings::IniFormat, &app);
  sessionSettings->beginGroup("sessions");
  RequestMapper::sessionStore = new HttpSessionStore(sessionSettings, &app);

  QSettings* fileSettings =
      new QSettings(configFileName, QSettings::IniFormat, &app);
  fileSettings->beginGroup("docroot");
  RequestMapper::staticFileController =
      new StaticFileController(fileSettings, &app);

  // Configure and start the TCP listener
  QSettings* listenerSettings =
      new QSettings(configFileName, QSettings::IniFormat, &app);
  listenerSettings->beginGroup("listener");
  listener = new HttpListener(listenerSettings, new RequestMapper(&app), &app);

  qWarning("Application has started");
  app.exec();

  qWarning("Application has stopped");
  return 0;
}

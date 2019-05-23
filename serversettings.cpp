#include <QCoreApplication>
#include <QDir>

#include "serversettings.h"

const QString ServerSettings::BASE_NAME = "clinic_queue";

QString ServerSettings::searchConfigFile() {
  QString binDir = QCoreApplication::applicationDirPath();
  QString appName = QCoreApplication::applicationName();
  QString fileName("config.ini");

  QStringList searchList;
  searchList.append(binDir);
  searchList.append(binDir + "/etc");
  searchList.append(binDir + "/../etc");
  searchList.append(binDir + "/../../etc");
  searchList.append(binDir + "/../" + appName + "/etc");
  searchList.append(binDir + "/../../" + appName + "/etc");
  searchList.append(binDir + "/../../../" + appName + "/etc");
  searchList.append(binDir + "/../../../../" + appName + "/etc");
  searchList.append(binDir + "/../../../../../" + appName + "/etc");
  searchList.append(QDir::rootPath() + "etc/opt");
  searchList.append(QDir::rootPath() + "etc");

  foreach (QString dir, searchList) {
    QFile file(dir + "/" + fileName);
    if (file.exists()) {
      // found
      fileName = QDir(file.fileName()).canonicalPath();
      qDebug("Using config file %s", qPrintable(fileName));
      return fileName;
    }
  }

  // not found
  foreach (QString dir, searchList) {
    qWarning("%s/%s not found", qPrintable(dir), qPrintable(fileName));
  }
  qFatal("Cannot find config file %s", qPrintable(fileName));
}

ServerSettings& ServerSettings::getInstance() {
  static ServerSettings instance;
  return instance;
}

ServerSettings::ServerSettings()
    : settings_file_(searchConfigFile()),
      settings_(settings_file_, QSettings::Format::IniFormat, qApp) {}

void ServerSettings::setConnectionSettings(const QString& serverAddr,
                                           const QString& userName,
                                           const QString& password) {
  settings_.beginGroup(kConnectionGroup);
  settings_.setValue(kServerAddress, serverAddr);
  settings_.setValue(kUserName, userName);
  settings_.setValue(kPassword, password);
  settings_.endGroup();
  settings_.sync();
}

QString ServerSettings::getPassword() const {
  return settings_.value(kConnectionGroup + "/" + kPassword).toString();
}

const QString& ServerSettings::getSettingsFile() const {
  return settings_file_;
}

QString ServerSettings::getUserName() const {
  return settings_.value(kConnectionGroup + "/" + kUserName).toString();
}

QString ServerSettings::getServerAddr() const {
  return settings_.value(kConnectionGroup + "/" + kServerAddress).toString();
}

int ServerSettings::getPort() const {
  return settings_.value(kConnectionGroup + "/" + kPort, 3306).toInt();
}

#ifdef QT_GUI_LIB
void AppSettings::setSizeSettings(const QByteArray& mainWindowGeometry,
                                  const QByteArray& mainWindowState) {
  settings_.beginGroup(SIZE_GROUP);
  settings_.setValue(MAIN_WINDOW_GEOMETRY, mainWindowGeometry);
  settings_.setValue(MAIN_WINDOW_STATE, mainWindowState);
  settings_.endGroup();
  settings_.sync();
}

QByteArray AppSettings::getMainWindowState() const {
  return settings_.value(SIZE_GROUP + "/" + MAIN_WINDOW_STATE).toByteArray();
}

QByteArray AppSettings::getMainWindowGeometry() const {
  return settings_.value(SIZE_GROUP + "/" + MAIN_WINDOW_GEOMETRY).toByteArray();
}
#endif

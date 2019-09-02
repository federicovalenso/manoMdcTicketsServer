#include "serversettings.h"
#include <QCoreApplication>
#include <QDir>
#include <QSettings>

namespace Settings {
QString searchConfigFile() {
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

void InitSettings() {
  auto app = qApp;
  app->setProperty(kSettingsFile, searchConfigFile());
  QSettings settings(qApp->property(kSettingsFile).toString(),
                     QSettings::Format::IniFormat, qApp);
  settings.beginGroup("/connetion");
  app->setProperty(kDbType, settings.value(kDbType).toString());
  app->setProperty(kServerAddress, settings.value(kServerAddress).toString());
  app->setProperty(kPort, settings.value(kPort, 3306).toInt());
  app->setProperty(kUserName, settings.value(kUserName).toString());
  app->setProperty(kPassword, settings.value(kPassword).toString());
  settings.endGroup();
}
}  // namespace Settings

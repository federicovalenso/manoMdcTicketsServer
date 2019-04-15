#ifndef SERVER_SETTINGS_H
#define SERVER_SETTINGS_H

#include <QByteArray>
#include <QSettings>
#include <QString>

class ServerSettings {
 public:
  static ServerSettings& getInstance();
  static QString searchConfigFile();

  void setConnectionSettings(const QString& serverAddr, const QString& baseName,
                             const QString& userName, const QString& password);
  void setSizeSettings(const QByteArray& mainWindowGeometry,
                       const QByteArray& mainWindowState);

  QString getServerAddr() const;
  QString getBaseName() const;
  QString getUserName() const;
  QString getPassword() const;
#ifdef QT_GUI_LIB
  QByteArray getMainWindowGeometry() const;
  QByteArray getMainWindowState() const;
#endif

  const QString& getSettingsFile() const;

 private:
  const QString CONNECTION_GROUP = "/connetion";
  const QString SERVER_ADDR = "server_addr";
  const QString BASE_NAME = "base_name";
  const QString USER_NAME = "user_name";
  const QString PASSWORD = "password";
  const QString SIZE_GROUP = "/size";
#ifdef QT_GUI_LIB
  const QString MAIN_WINDOW_GEOMETRY = "main_window_geometry";
  const QString MAIN_WINDOW_STATE = "main_window_state";
#endif

  QString settings_file_;
  QSettings settings_;

  ServerSettings();
  ServerSettings(const ServerSettings&) = delete;
  ServerSettings& operator=(ServerSettings&) = delete;
};

#endif  // SERVER_SETTINGS_H

#ifndef SERVER_SETTINGS_H
#define SERVER_SETTINGS_H

#include <QByteArray>
#include <QSettings>
#include <QString>

class ServerSettings {
 public:
  ServerSettings(const ServerSettings&) = delete;
  ServerSettings(ServerSettings&&) = delete;
  ServerSettings& operator=(const ServerSettings&) = delete;
  ServerSettings& operator=(ServerSettings&&) = delete;
  ~ServerSettings() = default;

  static ServerSettings& getInstance();
  static QString searchConfigFile();
  static const QString BASE_NAME;

  void setConnectionSettings(const QString& serverAddr, const QString& userName,
                             const QString& password);
  void setSizeSettings(const QByteArray& mainWindowGeometry,
                       const QByteArray& mainWindowState);

  QString getServerAddr() const;
  int getPort() const;
  QString getUserName() const;
  QString getPassword() const;

  const QString& getSettingsFile() const;

 private:
  const QString kConnectionGroup = "/connetion";
  const QString kServerAddress = "server_addr";
  const QString kPort = "port";
  const QString kUserName = "user_name";
  const QString kPassword = "password";
  const QString kSizeGroup = "/size";

  QString settings_file_;
  QSettings settings_;

  ServerSettings();
};

#endif  // SERVER_SETTINGS_H

#ifndef SERVER_SETTINGS_H
#define SERVER_SETTINGS_H

#include <QString>
#include <memory>

namespace Settings {
static const QString kBaseName = "clinic_queue";
static const QByteArray kSettingsFile = "settings_file";
static const QByteArray kServerAddress = "server_addr";
static const QByteArray kPort = "port";
static const QByteArray kUserName = "user_name";
static const QByteArray kPassword = "password";
static const QByteArray kDbType = "db_type";

void InitSettings();
};  // namespace Settings

#endif  // SERVER_SETTINGS_H

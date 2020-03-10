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
static const QByteArray kCounterGroup = "counter";
static const QByteArray kCurrentCount = "current_count";
static const QByteArray kPrevCount = "prev_count";
static const QByteArray kLastClearTime = "last_clear_time";
static const QByteArray kCriticalCount = "critical_count";
static const QByteArray kNotificationService = "notification_service";
static const QByteArray kNotificationMail = "mail";

void InitSettings();
};  // namespace Settings

#endif  // SERVER_SETTINGS_H

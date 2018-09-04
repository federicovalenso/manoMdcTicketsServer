#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QString>
#include <QByteArray>

class AppSettings
{
public:
    static AppSettings& getInstance();
    void setConnectionSettings(const QString& serverAddr,
                               const QString& baseName,
                               const QString& userName,
                               const QString& password);
    void setSizeSettings(const QByteArray& mainWindowGeometry,
                         const QByteArray& mainWindowState);

    QString getServerAddr() const;
    QString getBaseName() const;
    QString getUserName() const;
    QString getPassword() const;
    QByteArray getMainWindowGeometry() const;
    QByteArray getMainWindowState() const;

    inline int getCurEmployeeId() const {
        return mCurEmployeeId;
    }
    inline void setCurEmployeeId(int value) {
        mCurEmployeeId = value;
    }

private:
    AppSettings();
    AppSettings(const AppSettings&) = delete;
    AppSettings& operator=(AppSettings&) = delete;
    void ReadSettings();

    QSettings mSettings;
    const QString CONNECTION_GROUP = "/connetion";
    const QString SERVER_ADDR = "server_addr";
    const QString BASE_NAME = "base_name";
    const QString USER_NAME = "user_name";
    const QString PASSWORD = "password";
    const QString SIZE_GROUP = "/size";
    const QString MAIN_WINDOW_GEOMETRY = "main_window_geometry";
    const QString MAIN_WINDOW_STATE = "main_window_state";
    QString mServerAddr;
    QString mBaseName;
    QString mUserName;
    QString mPassword;
    QByteArray mMainWindowGeometry;
    QByteArray mMainWindowState;
    int mCurEmployeeId;
};

#endif // SETTINGS_H

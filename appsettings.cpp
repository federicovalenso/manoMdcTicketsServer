#include "appsettings.h"

AppSettings& AppSettings::getInstance()
{
    static AppSettings instance;
    return instance;
}

AppSettings::AppSettings()
{
    ReadSettings();
}

void AppSettings::setConnectionSettings(
        const QString& serverAddr,
        const QString& baseName,
        const QString& userName,
        const QString& password)
{
    mServerAddr = serverAddr;
    mBaseName = baseName;
    mUserName = userName;
    mPassword = password;
    mSettings.beginGroup(CONNECTION_GROUP);
    mSettings.setValue(SERVER_ADDR, serverAddr);
    mSettings.setValue(BASE_NAME, baseName);
    mSettings.setValue(USER_NAME, userName);
    mSettings.setValue(PASSWORD, password);
    mSettings.endGroup();
}

void AppSettings::setSizeSettings(const QByteArray &mainWindowGeometry, const QByteArray &mainWindowState)
{
    mMainWindowGeometry = mainWindowGeometry;
    mMainWindowState = mainWindowState;
    mSettings.beginGroup(SIZE_GROUP);
    mSettings.setValue(MAIN_WINDOW_GEOMETRY, mainWindowGeometry);
    mSettings.setValue(MAIN_WINDOW_STATE, mainWindowState);
    mSettings.endGroup();
}

QString AppSettings::getPassword() const
{
    return mPassword;
}

QString AppSettings::getUserName() const
{
    return mUserName;
}

QString AppSettings::getBaseName() const
{
    return mBaseName;
}

QString AppSettings::getServerAddr() const
{
    return mServerAddr;
}

QByteArray AppSettings::getMainWindowState() const
{
    return mMainWindowState;
}

QByteArray AppSettings::getMainWindowGeometry() const
{
    return mMainWindowGeometry;
}

void AppSettings::ReadSettings()
{
    mSettings.beginGroup(CONNECTION_GROUP);
    mServerAddr = mSettings.value(SERVER_ADDR).toString();
    mBaseName = mSettings.value(BASE_NAME).toString();
    mUserName = mSettings.value(USER_NAME).toString();
    mPassword = mSettings.value(PASSWORD).toString();
    mSettings.endGroup();
    mSettings.beginGroup(SIZE_GROUP);
    mMainWindowGeometry = mSettings.value(MAIN_WINDOW_GEOMETRY).toByteArray();
    mMainWindowState = mSettings.value(MAIN_WINDOW_STATE).toByteArray();
    mSettings.endGroup();
}

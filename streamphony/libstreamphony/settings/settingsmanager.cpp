#include "settingsmanager.h"

#include <QStandardPaths>
#include <QDebug>

SettingsManager *SettingsManager::s_instance = nullptr;

static const QString SETTINGS_FILE = QStringLiteral("/settings.ini");

//Keys
static const QString EMAIL_KEY = "email";
static const QString PASSWORD_KEY = "password";
static const QString XMPP_USERNAME_KEY = "xmppUsername";
static const QString HTTP_PORT_KEY = "httpPort";
static const QString SHARED_PATHS_KEY = "sharedPaths";

//Debug

#define DEBUG_SETTINGS

#ifdef DEBUG_SETTINGS
    #define debugSettings qDebug
#else
    #define debugSettings QT_NO_QDEBUG_MACRO
#endif


SettingsManager::SettingsManager(QObject *parent) :
    QObject(parent)
{
    Q_ASSERT(s_instance == nullptr);
    s_instance = this;

    debugSettings() << "Settings location:" << QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + SETTINGS_FILE;

    m_settings.reset(new QSettings(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + SETTINGS_FILE,
                                   QSettings::IniFormat));
}

SettingsManager::~SettingsManager()
{
    s_instance = nullptr;
}

SettingsManager *SettingsManager::instance()
{
    Q_ASSERT(s_instance != nullptr);
    return s_instance;
}

QString SettingsManager::email() const
{
    return m_settings->value(EMAIL_KEY, {}).toString();
}

void SettingsManager::setEmail(const QString &newEmail)
{
    if (newEmail != email()) {
        m_settings->setValue(EMAIL_KEY, newEmail);
        emit emailChanged(newEmail);
    }
}

QString SettingsManager::password() const
{
    return m_settings->value(PASSWORD_KEY, {}).toString();
}

void SettingsManager::setPassword(const QString &newPassword)
{
    if (newPassword != password()) {
        m_settings->setValue(PASSWORD_KEY, newPassword);
        emit passwordChanged(newPassword);
    }
}

QString SettingsManager::xmppUsername() const
{
    return m_settings->value(XMPP_USERNAME_KEY, {}).toString();
}

void SettingsManager::setXmppUsername(const QString &newXmppUsername)
{
    if (newXmppUsername != xmppUsername()) {
        m_settings->setValue(XMPP_USERNAME_KEY, newXmppUsername);
        emit xmppUsernameChanged(newXmppUsername);
    }
}

quint32 SettingsManager::httpPort() const
{
    return m_settings->value(HTTP_PORT_KEY, {}).toUInt();
}

void SettingsManager::setHttpPort(quint32 port)
{
    if (port != httpPort()) {
        m_settings->setValue(HTTP_PORT_KEY, QVariant(port));
        emit httpPortChanged(port);
    }
}

QStringList SettingsManager::sharedPaths() const
{
    return m_settings->value(SHARED_PATHS_KEY, {}).toStringList();
}

void SettingsManager::setSharedPaths(const QStringList &newValue)
{
    if (newValue != sharedPaths()) {
        m_settings->setValue(SHARED_PATHS_KEY, newValue);
        emit sharedPathsChanged(newValue);
    }
}

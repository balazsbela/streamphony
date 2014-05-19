#include "settingsmanager.h"

#include <QStandardPaths>
#include <QDebug>

SettingsManager *SettingsManager::s_instance = nullptr;

static const QString SETTINGS_FILE = QStringLiteral("/settings.ini");

//Keys
static const QString EMAIL_KEY = "email";
static const QString PASSWORD_KEY = "password";

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

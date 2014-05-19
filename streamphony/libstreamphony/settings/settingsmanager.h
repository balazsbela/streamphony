#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QObject>
#include <QSettings>
#include <QScopedPointer>

class SettingsManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString email READ email WRITE setEmail NOTIFY emailChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)

public:
    explicit SettingsManager(QObject *parent = 0);
    ~SettingsManager() override;

    static SettingsManager *instance();

    QString email() const;
    void setEmail(const QString &value);

    QString password() const;
    void setPassword(const QString &value);

signals:
    void emailChanged(const QString &newValue);
    void passwordChanged(const QString &newValue);

private:
    static SettingsManager *s_instance;
    QScopedPointer<QSettings> m_settings;
};

#endif // SETTINGSMANAGER_H

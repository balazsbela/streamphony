#include "daemon/lighthttpdaemon.h"
#include "daemon/localfilecontentresolver.h"
#include "daemon/portrange.h"
#include "settings/settingsmanager.h"

#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    SettingsManager settingsManager(&app);

    LocalFileContentResolver *resolver = new LocalFileContentResolver(&app);
    LightHttpDaemon daemon(MIN_PORT, MIN_PORT, MAX_PORT, &app);
    daemon.setContentResolver(resolver);

    return app.exec();
}

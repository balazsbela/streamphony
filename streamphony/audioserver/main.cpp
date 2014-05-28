#include "daemon/lighthttpdaemon.h"
#include "daemon/localfilecontentresolver.h"
#include "daemon/portrange.h"

#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    LocalFileContentResolver *resolver = new LocalFileContentResolver(&app);
    LightHttpDaemon daemon(MIN_PORT+3, MIN_PORT, MAX_PORT, &app);
    daemon.setContentResolver(resolver);

    return app.exec();
}

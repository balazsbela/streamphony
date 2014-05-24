#include "daemon/lighthttpdaemon.h"
#include "daemon/localfilecontentresolver.h"

#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    LocalFileContentResolver *resolver = new LocalFileContentResolver(&app);
    LightHttpDaemon daemon(8081);
    daemon.setContentResolver(resolver);

    return app.exec();
}

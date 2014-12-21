#include <QApplication>
#include "streamerfrontend.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    StreamerFrontend streamer;

    return app.exec();
}

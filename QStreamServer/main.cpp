#include "streamserver.h"
#include <QApplication>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setApplicationDisplayName("QStreamServer");
    QApplication::setApplicationName(QApplication::applicationDisplayName());
    QApplication::setApplicationVersion("0.1");

    QString configFile;
    QCommandLineParser *cParse = new QCommandLineParser();
    cParse->addPositionalArgument("config",QApplication::tr("Configuration file"),"[config-file.json]");
    cParse->process(a);
    if(cParse->positionalArguments().size()<1){
        printf(QApplication::tr("Server failed to start: No configuration file provided.\n").toLocal8Bit());
        return 1;
    } else
        configFile = cParse->positionalArguments().at(0);

    StreamServer w(configFile);
    w.show();

    return a.exec();
}

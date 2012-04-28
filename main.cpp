#include <QtCore/QCoreApplication>

#include "autoconnect.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    AutoConnect ac;
    if (ac.init())
        return a.exec();
}

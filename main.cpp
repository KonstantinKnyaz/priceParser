#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName(ORGANIZATION_NAME);
    QCoreApplication::setOrganizationDomain("https://vk.com/id15631385");
    QCoreApplication::setApplicationName(APP_NAME);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

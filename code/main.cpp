#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("MyOrg");
    QCoreApplication::setApplicationName("sd-desktop-assistant");
    Widget w;
    w.show();
    return a.exec();
}

#include <QApplication>
#include "basewindow.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("Panopticon");
    a.setOrganizationDomain("tanwts.altervista.org");
    a.setApplicationName("Infinity Translator");

    Preferences *prefs = Preferences::getInstance();
    prefs->loadPreferences();

    BaseWindow w;
    w.show();

    return a.exec();
}

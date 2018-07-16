#include "preferences.hpp"

Preferences *Preferences::singleton = NULL;

void Preferences::init() {
    if (singleton == NULL)
        singleton = new Preferences();
}

Preferences::Preferences() {
    settings = new QSettings();
}

Preferences::~Preferences() {
    delete settings;
}

Preferences *Preferences::getInstance() {
    init();

    return singleton;
}

void Preferences::loadPreferences() {
    QString ver = retrieveVersion();

    if (ver != VERSION) {
        settings->clear();
        saveVersion(VERSION);
    }
}

void Preferences::saveRecentDestFile(QStringList files) {
    while (files.size() > (MaxRecentFiles / 2))
        files.removeLast();

    settings->setValue(RECENTDESTFILESKEY, files);
}

QStringList Preferences::retrieveRecentDestFile() {
    return settings->value(RECENTDESTFILESKEY).toStringList();
}

void Preferences::saveRecentSourceFile(QStringList files) {
    while (files.size() > (MaxRecentFiles / 2))
        files.removeLast();

    settings->setValue(RECENTSOURCEFILESKEY, files);
}

QStringList Preferences::retrieveRecentSourceFile() {
    return settings->value(RECENTSOURCEFILESKEY).toStringList();
}

void Preferences::saveVersion(QString ver){
    settings->setValue(VERSIONKEY, ver);
}

QString Preferences::retrieveVersion(){
    return settings->value(VERSIONKEY).toString();
}

void Preferences::clearRecents() {
    settings->remove(RECENTSOURCEFILESKEY);
    settings->remove(RECENTDESTFILESKEY);
}

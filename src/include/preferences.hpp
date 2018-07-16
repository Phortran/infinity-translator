#ifndef PREFERENCES_HPP
#define PREFERENCES_HPP

#include <QString>
#include <QStringList>
#include <QSettings>

#define VERSION "1.1.3"

static const QString RECENTDESTFILESKEY = "RecentDestFiles";
static const QString RECENTSOURCEFILESKEY = "RecentSourceFiles";
static const QString VERSIONKEY = "version";

class Preferences {
private:
    static Preferences *singleton;
    QSettings *settings;

    static void init();
    void saveVersion(QString ver);

protected:
    Preferences();

public:
    // this should be even, since it is the total number of recent
    // files per type (and there are two types): first half will be
    // of the one type, while the second half will be of the other.
    static const int MaxRecentFiles = 10;

    static Preferences *getInstance();
    void loadPreferences();

    void saveRecentDestFile(QStringList files);
    QStringList retrieveRecentDestFile();
    void saveRecentSourceFile(QStringList files);
    QStringList retrieveRecentSourceFile();
    QString retrieveVersion();
    void clearRecents();

    ~Preferences();
};

#endif // PREFERENCES_HPP

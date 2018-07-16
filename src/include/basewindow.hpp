#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

#include "trahandler.hpp"
#include "ui_basewindow.h"
#include "stringitem.hpp"
#include "preferences.hpp"

namespace Ui {
class BaseWindow;
}

class BaseWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BaseWindow(QWidget *parent = 0);
    ~BaseWindow();

private slots:
    void on_actionQuit_triggered();
    void on_actionAuthor_triggered();
    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void on_transText_textChanged();
    void on_actionSaveAs_triggered();
    void on_action_Save_triggered();
    void on_actionAbout_Qt_triggered();
    void on_action_open_source_file_triggered();
    void on_action_open_destination_file_triggered();
    void openRecentSourceFile();
    void openRecentDestFile();
    void clearRecentFiles();

private:
    Ui::BaseWindow  *ui;
    TRAHandler      *tra;
    bool            isDirty;
    QString         loadedSourceFilename;
    QString         savedDestFilename;
    QAction         *recentSourceFileActs[(Preferences::MaxRecentFiles / 2)];
    QAction         *recentDestFileActs[(Preferences::MaxRecentFiles / 2)];
    QAction         *upperSeparatorAct;
    QAction         *midSeparatorAct;
    QAction         *bottomSeparatorAct;
    QAction         *clearAct;

    void saveFile();
    void loadRecent();
    void updateRecentFileActions();
    void setCurrentSourceFile();
    void setCurrentDestFile();
    void loadSourceFile(QString filename);
    void loadDestFile(QString filename);
};

#endif // BASEWINDOW_H

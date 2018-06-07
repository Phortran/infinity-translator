#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

#include "trahandler.hpp"

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

    //void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_transText_textChanged();

    void on_actionSaveAs_triggered();

    void on_action_Save_triggered();

    void on_actionAbout_Qt_triggered();

    void on_action_open_source_file_triggered();

    void on_action_open_destination_file_triggered();

private:
    Ui::BaseWindow *ui;
    TRAHandler *tra;
    bool isDirty;
    QString savedFilename;

    void saveFile();
};

#endif // BASEWINDOW_H

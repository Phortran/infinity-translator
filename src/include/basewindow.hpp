#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

#include "tra.hpp"

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
    void on_actionOpen_triggered();

    void on_actionQuit_triggered();

    void on_actionAuthor_triggered();

    //void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

private:
    Ui::BaseWindow *ui;
    //QGridLayout *stringLayout;
    //QQueue<StringItem *> stringItems;
    TRAHandler *tra;
};

#endif // BASEWINDOW_H

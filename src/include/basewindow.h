#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

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

private:
    Ui::BaseWindow *ui;
    QGridLayout *stringLayout;
};

#endif // BASEWINDOW_H

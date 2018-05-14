#include "basewindow.h"
#include "ui_basewindow.h"
#include "stringitem.h"

BaseWindow::BaseWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BaseWindow)
{
    ui->setupUi(this);
    //stringLayout = new QGridLayout;
    //ui->stringsGroupBox->setLayout(stringLayout);
}

BaseWindow::~BaseWindow()
{
    delete ui;
}

void BaseWindow::on_actionOpen_triggered()
{
    //static int count = 0;

    QMessageBox::information(this, tr("Test dialog"),
                             tr("Open file."));
    //this->stringItems.enqueue(new StringItem);
    //this->stringLayout->addWidget(this->stringItems.last(), count, 0);
    //count++;
}

void BaseWindow::on_actionQuit_triggered()
{
    delete ui;

    //foreach (StringItem * item, this->stringItems) {
    //    this->stringLayout->removeWidget(item);
    //    delete item;
    //}

    exit(0);
}

void BaseWindow::on_actionAuthor_triggered()
{
    QMessageBox::information(this, tr("About the author"),
                             tr("About the author."));
}

#include "basewindow.hpp"
#include "ui_basewindow.h"
#include "stringitem.hpp"

BaseWindow::BaseWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BaseWindow)
{
    ui->setupUi(this);
    ui->statusBar->showMessage("Welcome to Infinity translator!");
    //stringLayout = new QGridLayout;
    //ui->stringsGroupBox->setLayout(stringLayout);
    this->tra = NULL;
}

BaseWindow::~BaseWindow()
{
    delete ui;
}

void BaseWindow::on_actionOpen_triggered() {
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Infinity language file"),
                                                    "./",
                                                    tr("Text files (*.tra *.txt)"));

    if (fileName != NULL) {
        ui->statusBar->showMessage("Loading file...");
        if (this->tra != NULL)
            delete this->tra;
        this->tra = new TRAHandler(fileName);

        try {
            this->tra->init();
        }
        catch (const std::exception &e) {
            ui->statusBar->showMessage(e.what());
            return;
        }

        //TODO populate list view
        new QListWidgetItem(tr("Stocazzo"), ui->listWidget);
        ui->statusBar->showMessage("File correctly loaded!", 5000);
    }
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

//void BaseWindow::on_listWidget_itemClicked(QListWidgetItem *item)
//{

//}

void BaseWindow::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    ui->statusBar->showMessage("Item clicked!", 5000);
}

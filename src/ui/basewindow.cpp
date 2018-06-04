#include "basewindow.hpp"
#include "ui_basewindow.h"
#include "stringitem.hpp"
#include "utils.hpp"

BaseWindow::BaseWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BaseWindow) {

    ui->setupUi(this);
    ui->statusBar->showMessage("Welcome to Infinity translator!");
    //stringLayout = new QGridLayout;
    //ui->stringsGroupBox->setLayout(stringLayout);
    this->tra = NULL;
    this->isDirty = false;
    this->savedFilename = "NA";
    this->savedFile = NULL;

    this->ui->transText->setEnabled(false);
}

BaseWindow::~BaseWindow() {
    delete ui;
}

void BaseWindow::on_actionOpen_triggered() {
    StringList strings;
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Infinity language file"),
                                                    "./",
                                                    tr("Text files (*.tra);;All Files (*)"));

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
            this->ui->transText->setEnabled(false);
            return;
        }

        this->ui->transText->setEnabled(true);

        strings = this->tra->getOrigStrings();

        foreach (const StringItem &item, strings) {
            new QListWidgetItem(item.getTextIndex(), ui->listWidget);
        }

        ui->statusBar->showMessage("File correctly loaded!", 5000);
    }
}

void BaseWindow::on_actionQuit_triggered() {
    delete ui;

    //foreach (StringItem * item, this->stringItems) {
    //    this->stringLayout->removeWidget(item);
    //    delete item;
    //}

    if (this->isDirty) {
        if (QMessageBox::question(this, tr("Save the work?"),
                                  tr("Some strings were modified;"
                                     "do you want to save the file?")) == QMessageBox::Yes) {
            //TODO
            printf("DEBUG: Saving file\n");
            this->on_action_Save_triggered();
        }
        else {
            printf("DEBUG: File discarded\n");
        }
    }

    exit(0);
}

void BaseWindow::on_actionAuthor_triggered() {
    QMessageBox::about(this, tr("About the author"),
                             tr("About the author."));
}

void BaseWindow::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous) {
    int index;

    if (this->ui->transText->document()->isModified()) {
        QString toBeSaved = ui->transText->toPlainText();
        index = ui->listWidget->row(previous);
        if (index >= 0)
            this->tra->setStringAt(index, toBeSaved);
    }

    index = ui->listWidget->row(current);
    QString origString = this->tra->getStringItemAt(index, TRAHandler::ORIG).getText();
    QString tranString = this->tra->getStringItemAt(index, TRAHandler::TRAN).getText();

    ui->origText->setText(origString);
    ui->transText->setText(tranString);
}

void BaseWindow::on_transText_textChanged()
{
    if (!this->isDirty && this->ui->transText->document()->isModified())
        this->isDirty = true;
}

void BaseWindow::saveFile()
{
    //TODO
    printf("DEBUG: saveFile called\n");

    ui->statusBar->showMessage("Saving file...");

    if (this->ui->transText->document()->isModified()) {
        QString toBeSaved = ui->transText->toPlainText();
        int index = ui->listWidget->currentRow();
        if (index >= 0)
            this->tra->setStringAt(index, toBeSaved);
    }

    if (this->savedFilename == "NA") {
        // should NEVER happen
        fprintf(stderr, "WARNING: saveFile called without a valid file name\n");
        ui->statusBar->showMessage("File not saved!", 5000);
        return;
    }

    this->savedFile = new QFile(this->savedFilename);

    if (this->savedFile->open(QIODevice::WriteOnly)) {
        QTextStream stream(this->savedFile);

        foreach (StringItem item, this->tra->getTranStrings()) {
            stream << "@" << item.getIndex() << "=~" << item.getText() << "~" << endl << endl;
        }
    }
    else {
        QMessageBox::warning(this, tr("Error during opening of file"),
                             tr("Unable to open file\"") + this->savedFilename + tr("\"for saving:")
                             + "\n" + this->savedFile->errorString());
        ui->statusBar->showMessage("File not saved!", 5000);
        return;
    }

    this->isDirty = false;
    ui->statusBar->showMessage("File correctly saved!", 5000);
}

void BaseWindow::on_actionSaveAs_triggered()
{
    this->savedFilename = QFileDialog::getSaveFileName(this,
                                                   tr("Save TRA file"), "",
                                                   tr("Infinity TRA file (*.tra);;All Files (*)"));

    this->saveFile();
}

void BaseWindow::on_action_Save_triggered()
{
    if (this->savedFile == NULL || this->savedFilename == "NA")
        this->on_actionSaveAs_triggered();
    else {
        this->saveFile();
    }
}

void BaseWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this, tr("About Qt"));
}

#include "basewindow.hpp"
#include "ui_basewindow.h"
#include "stringitem.hpp"
#include "utils.hpp"

BaseWindow::BaseWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BaseWindow) {

    ui->setupUi(this);
    ui->statusBar->showMessage("Welcome to Infinity translator!");
    this->tra = NULL;
    this->isDirty = false;
    this->savedFilename = "NA";

    this->ui->transText->setEnabled(false);
}

BaseWindow::~BaseWindow() {
    if (this->tra != NULL)
        delete this->tra;
    delete ui;
}

void BaseWindow::on_actionQuit_triggered() {
    delete ui;

    if (this->isDirty) {
        if (QMessageBox::question(this, tr("Save the work?"),
                                  tr("Some strings were modified;"
                                     "do you want to save the file?")) == QMessageBox::Yes) {
            this->on_action_Save_triggered();
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

void BaseWindow::saveFile() {
    QFile savedFile(this->savedFilename);

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

    if (!savedFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QMessageBox::warning(this, tr("Error during opening of file"),
                             tr("Unable to open file\"") + this->savedFilename + tr("\"for saving:")
                             + "\n" + savedFile.errorString());
        ui->statusBar->showMessage("File not saved!", 5000);
        return;
    }

    if (savedFile.isWritable()) {
        QTextStream stream(&savedFile);
        StringList tranStrings = this->tra->getTranStrings();
        StringList::const_iterator iter;

        for (iter = tranStrings.begin(); iter != tranStrings.end(); ++iter) {
            if (iter != tranStrings.begin())
                stream << endl;
            stream << "@" << iter->getIndex() << "=~" << iter->getText() << "~" << endl;
        }
    }
    else {
        QMessageBox::warning(this, tr("Error during opening of file"),
                             tr("Unable to open file\"") + this->savedFilename + tr("\"for saving:")
                             + "\n" + savedFile.errorString());
        ui->statusBar->showMessage("File not saved!", 5000);
        savedFile.close();
        return;
    }

    this->isDirty = false;
    savedFile.close();
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
    if (this->savedFilename == "NA")
        this->on_actionSaveAs_triggered();
    else {
        this->saveFile();
    }
}

void BaseWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this, tr("About Qt"));
}

void BaseWindow::on_action_open_source_file_triggered()
{
    StringList strings;
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Infinity language file"),
                                                    "./",
                                                    tr("Text files (*.tra);;All Files (*)"));

    if (fileName != NULL) {
        ui->statusBar->showMessage("Loading file...");
        if (this->tra != NULL) {
            if (this->isDirty) {
                if (QMessageBox::question(this, tr("Save the work?"),
                                          tr("Some strings were modified;"
                                             "do you want to save the file?")) == QMessageBox::Yes) {
                    this->on_action_Save_triggered();
                }
            }
            delete this->tra;
        }
        this->tra = new TRAHandler(fileName);

        try {
            this->tra->init();
        }
        catch (const std::exception &e) {
            ui->statusBar->showMessage(e.what());
            this->ui->transText->setEnabled(false);
            this->ui->action_open_destination_file->setEnabled(false);
            return;
        }

        this->ui->transText->setEnabled(true);
        this->ui->action_open_destination_file->setEnabled(true);

        strings = this->tra->getOrigStrings();

        foreach (const StringItem &item, strings) {
            new QListWidgetItem(item.getTextIndex(), ui->listWidget);
        }

        ui->statusBar->showMessage("Source file correctly loaded!", 5000);
    }
}

void BaseWindow::on_action_open_destination_file_triggered()
{
    StringList strings;
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Infinity language file"),
                                                    "./",
                                                    tr("Text files (*.tra);;All Files (*)"));

    if (fileName != NULL) {
        ui->statusBar->showMessage("Loading file...");
        if (this->tra != NULL) {
            if (this->isDirty) {
                if (QMessageBox::question(this, tr("Save the work?"),
                                          tr("Some strings were modified;"
                                             "do you want to save the work in "
                                             "the previously opened file?")) == QMessageBox::Yes) {
                    this->on_action_Save_triggered();
                }
            }
        }
        QString oldDestFilePath = this->tra->getDestFilePath();
        this->tra->setDestFilePath(fileName);

        try {
            this->tra->setTranStrings();
        }
        catch (const std::exception &e) {
            ui->statusBar->showMessage(e.what());
            this->tra->setDestFilePath(oldDestFilePath);
            return;
        }

        this->savedFilename = fileName;
        ui->statusBar->showMessage("Translated file correctly loaded!", 5000);
    }
}

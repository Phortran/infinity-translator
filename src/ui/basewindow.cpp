#include "basewindow.hpp"

BaseWindow::BaseWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BaseWindow) {

    ui->setupUi(this);
    ui->statusBar->showMessage("Welcome to Infinity translator!");
    tra = NULL;
    isDirty = false;
    savedDestFilename = "NA";
    loadedSourceFilename = "NA";

    ui->transText->setEnabled(false);

    loadRecent();
}

BaseWindow::~BaseWindow() {
    if (tra != NULL)
        delete tra;
    delete ui;
}

void BaseWindow::on_actionQuit_triggered() {
    delete ui;

    if (isDirty) {
        if (QMessageBox::question(this, tr("Save the work?"),
                                  tr("Some strings were modified;"
                                     "do you want to save the file?")) == QMessageBox::Yes) {
            on_action_Save_triggered();
        }
    }

    exit(0);
}

void BaseWindow::on_actionAuthor_triggered() {
    QMessageBox::about(this, tr("About the author"),
                             tr("Released under the MIT Licence v3.\n"
                                "Copyright 2018, <b>Gabriele Santi</b>."));
}

void BaseWindow::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous) {
    int index;

    if (ui->transText->document()->isModified()) {
        QString toBeSaved = ui->transText->toPlainText();
        index = ui->listWidget->row(previous);
        if (index >= 0)
            tra->setStringAt(index, toBeSaved);
    }

    index = ui->listWidget->row(current);
    QString origString = tra->getStringItemAt(index, TRAHandler::ORIG).getText();
    QString tranString = tra->getStringItemAt(index, TRAHandler::TRAN).getText();

    ui->origText->setText(origString);
    ui->transText->setText(tranString);
}

void BaseWindow::on_transText_textChanged()
{
    if (!isDirty && ui->transText->document()->isModified())
        isDirty = true;
}

void BaseWindow::saveFile() {
    QFile savedFile(savedDestFilename);

    ui->statusBar->showMessage("Saving file...");

    // retrieve currently visible string, if modified
    if (ui->transText->document()->isModified()) {
        QString toBeSaved = ui->transText->toPlainText();
        int index = ui->listWidget->currentRow();
        if (index >= 0)
            tra->setStringAt(index, toBeSaved);
    }

    if (savedDestFilename == "NA") {
        // should NEVER happen
        fprintf(stderr, "WARNING: saveFile called without a valid file name\n");
        ui->statusBar->showMessage("File not saved!", 5000);
        return;
    }

    if (!savedFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QMessageBox::warning(this, tr("Error during opening of file"),
                             tr("Unable to open file\"") + savedDestFilename + tr("\"for saving:")
                             + "\n" + savedFile.errorString());
        ui->statusBar->showMessage("File not saved!", 5000);
        return;
    }

    if (savedFile.isWritable()) {
        QTextStream stream(&savedFile);
        StringList tranStrings = tra->getTranStrings();
        StringList::const_iterator iter;

        for (iter = tranStrings.begin(); iter != tranStrings.end(); ++iter) {
            if (iter != tranStrings.begin())
                stream << endl;
            stream << "@" << iter->getIndex() << "=~" << iter->getText() << "~" << endl;
        }
    }
    else {
        QMessageBox::warning(this, tr("Error during opening of file"),
                             tr("Unable to write on file\"") + savedDestFilename + tr("\":")
                             + "\n" + savedFile.errorString());
        ui->statusBar->showMessage("File not saved!", 5000);
        savedFile.close();
        return;
    }

    isDirty = false;
    savedFile.close();
    ui->statusBar->showMessage("File correctly saved!", 5000);
}

/**
 * @brief BaseWindow::loadRecent
 *
 * Load recent files
 * TODO: technically this operation should be done in QML
 *       in Design mode.
 */
void BaseWindow::loadRecent() {
    int halfMaxRecent = (Preferences::MaxRecentFiles / 2);

    for (int i = 0; i < Preferences::MaxRecentFiles; ++i) {
        // load source files actions
        if (i < halfMaxRecent) {
            recentSourceFileActs[i] = new QAction(this);
            recentSourceFileActs[i]->setVisible(false);

            connect(recentSourceFileActs[i], SIGNAL(triggered()),
                    this, SLOT(openRecentSourceFile()));
        } // load dest files actions
        else {
            recentDestFileActs[i-halfMaxRecent] = new QAction(this);
            recentDestFileActs[i-halfMaxRecent]->setVisible(false);

            connect(recentDestFileActs[i-halfMaxRecent], SIGNAL(triggered()),
                    this, SLOT(openRecentDestFile()));
        }
    }

    // upper half of recent entries are for source files
    upperSeparatorAct = ui->menu_Open->addSeparator();
    for (int i = 0; i < halfMaxRecent; ++i)
        ui->menu_Open->addAction(recentSourceFileActs[i]);

    // lower half of recent entries are for destination files
    midSeparatorAct = ui->menu_Open->addSeparator();
    for (int i = 0; i < halfMaxRecent; ++i)
        ui->menu_Open->addAction(recentDestFileActs[i]);

    // add "clear recents"
    bottomSeparatorAct = ui->menu_Open->addSeparator();
    bottomSeparatorAct->setVisible(true);
    clearAct = new QAction(this);
    clearAct->setVisible(true);
    connect(clearAct, SIGNAL(triggered()), this, SLOT(clearRecentFiles()));
    clearAct->setText(tr("&Clear recents"));
    ui->menu_Open->addAction(clearAct);

    updateRecentFileActions();
}

void BaseWindow::updateRecentFileActions() {
    QStringList sourceFiles = Preferences::getInstance()->retrieveRecentSourceFile();
    QStringList destFiles = Preferences::getInstance()->retrieveRecentDestFile();

    int numRecentSourceFiles = qMin(sourceFiles.size(), (Preferences::MaxRecentFiles / 2));
    int numRecentDestFiles = qMin(destFiles.size(), (Preferences::MaxRecentFiles / 2));
    int totalNumRecentFiles = numRecentSourceFiles + numRecentDestFiles;

    // set recent source files
    for (int i = 0; i < numRecentSourceFiles; ++i) {
        QString text = tr("&%1 %2").arg(i + 1).arg(QFileInfo(sourceFiles[i]).fileName());
        recentSourceFileActs[i]->setText(text);
        recentSourceFileActs[i]->setData(sourceFiles[i]);
        recentSourceFileActs[i]->setVisible(true);
    }
    // set the rest of recent entries invisible, if applicable)
    for (int j = numRecentSourceFiles; j < (Preferences::MaxRecentFiles / 2); ++j)
        recentSourceFileActs[j]->setVisible(false);

    // set recent destination files
    for (int i = numRecentSourceFiles, j = 0; i < totalNumRecentFiles; ++i, ++j) {
        QString text = tr("&%1 %2").arg(i + 1).arg(QFileInfo(destFiles[j]).fileName());
        recentDestFileActs[j]->setText(text);
        recentDestFileActs[j]->setData(destFiles[j]);
        recentDestFileActs[j]->setVisible(true);
    }
    // set the rest of recent entries invisible, if applicable)
    for (int j = numRecentDestFiles; j < (Preferences::MaxRecentFiles / 2); ++j)
        recentDestFileActs[j]->setVisible(false);

    upperSeparatorAct->setVisible(totalNumRecentFiles > 0);
    midSeparatorAct->setVisible(numRecentSourceFiles != 0 && numRecentDestFiles != 0);
    clearAct->setEnabled(totalNumRecentFiles > 0);
}

void BaseWindow::on_actionSaveAs_triggered()
{
    savedDestFilename = QFileDialog::getSaveFileName(this,
                                                   tr("Save TRA file"), "",
                                                   tr("Infinity TRA file (*.tra);;All Files (*)"));

    setCurrentDestFile();
    saveFile();
}

void BaseWindow::openRecentSourceFile() {
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
        loadSourceFile(action->data().toString());
}

void BaseWindow::openRecentDestFile() {
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
        loadDestFile(action->data().toString());
}

void BaseWindow::on_action_Save_triggered()
{
    if (savedDestFilename == "NA")
        on_actionSaveAs_triggered();
    else {
        saveFile();
    }
}

void BaseWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this, tr("About Qt"));
}

void BaseWindow::clearRecentFiles() {
    Preferences *prefs = Preferences::getInstance();
    prefs->clearRecents();

    updateRecentFileActions();
}

void BaseWindow::on_action_open_source_file_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Infinity language file"),
                                                    "./",
                                                    tr("Text files (*.tra);;All Files (*)"));

    loadSourceFile(fileName);
}

void BaseWindow::loadSourceFile(QString filename) {
    if (filename != NULL) {
        ui->statusBar->showMessage("Loading file...");
        if (tra != NULL) {
            if (isDirty) {
                if (QMessageBox::question(this, tr("Save the work?"),
                                          tr("Some strings were modified;"
                                             "do you want to save the file?")) == QMessageBox::Yes) {
                    on_action_Save_triggered();
                }
            }
            delete tra;
        }
        tra = new TRAHandler(filename);

        try {
            tra->init();
        }
        catch (const std::exception &e) {
            ui->statusBar->showMessage(e.what());
            ui->transText->setEnabled(false);
            ui->action_open_destination_file->setEnabled(false);
            return;
        }

        ui->transText->setEnabled(true);
        ui->action_open_destination_file->setEnabled(true);

        StringList strings = tra->getOrigStrings();

        foreach (const StringItem &item, strings) {
            new QListWidgetItem(item.getTextIndex(), ui->listWidget);
        }

        loadedSourceFilename = filename;
        setCurrentSourceFile();
        ui->statusBar->showMessage("Source file correctly loaded!", 5000);
    }
}

void BaseWindow::on_action_open_destination_file_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Infinity language file"),
                                                    "./",
                                                    tr("Text files (*.tra);;All Files (*)"));

    loadDestFile(fileName);
}

void BaseWindow::loadDestFile(QString filename) {
    if (tra == NULL) {
        QMessageBox::warning(this, tr("Error during opening of file"),
                             tr("It seems you are trying to open a destination file "
                                      "without having first opened a source file; open the"
                                      " latter, first.\n"));
        return;
    }

    if (filename != NULL) {
        ui->statusBar->showMessage("Loading file...");
            if (isDirty) {
                if (QMessageBox::question(this, tr("Save the work?"),
                                          tr("Some strings were modified;"
                                             "do you want to save the work in "
                                             "the previously opened file?")) == QMessageBox::Yes) {
                    on_action_Save_triggered();
                }
            }
        QString oldDestFilePath = tra->getDestFilePath();
        tra->setDestFilePath(filename);

        try {
            tra->setTranStrings();
        }
        catch (const std::exception &e) {
            ui->statusBar->showMessage(e.what());
            tra->setDestFilePath(oldDestFilePath);
            return;
        }

        savedDestFilename = filename;
        setCurrentDestFile();
        ui->statusBar->showMessage("Translated file correctly loaded!", 5000);
    }
}

void BaseWindow::setCurrentDestFile() {
    if (savedDestFilename == "NA") {
        // this should NEVER happen
        fprintf(stderr, "WARNING: setCurrentDestFile called without a valid file name\n");
        return;
    }

    Preferences *prefs = Preferences::getInstance();
    QStringList files = prefs->retrieveRecentDestFile();
    files.removeAll(savedDestFilename);
    files.prepend(savedDestFilename);
    prefs->saveRecentDestFile(files);

    updateRecentFileActions();
}

void BaseWindow::setCurrentSourceFile() {
    if (loadedSourceFilename == "NA") {
        // this should NEVER happen
        fprintf(stderr, "WARNING: setCurrentSourceFile called without a valid file name\n");
        return;
    }

    Preferences *prefs = Preferences::getInstance();
    QStringList files = prefs->retrieveRecentSourceFile();
    files.removeAll(loadedSourceFilename);
    files.prepend(loadedSourceFilename);
    prefs->saveRecentSourceFile(files);

    updateRecentFileActions();
}

#include "trahandler.hpp"

StringList TRAHandler::getOrigStrings() const
{
    return origStrings;
}

StringItem TRAHandler::getStringItemAt(int index, bool which)
{
    if (which)
        return this->origStrings.at(index);
    else
        return this->tranStrings.at(index);
}

StringList TRAHandler::getTranStrings() const
{
    return tranStrings;
}

void TRAHandler::setStringAt(int index, QString data)
{
    StringItem _new = this->tranStrings.at(index);
    _new.setText(data);

    this->tranStrings.replace(index, _new);
}

void TRAHandler::setTranStrings() {
    if (this->destFilePath == NULL || this->destFilePath == "")
        throw std::invalid_argument("Invalid destination filename");

    StringList *list = extractStringItemsFromFile(this->destFilePath);

    if (this->tranStrings.size() != list->size())
        throw std::length_error("Incoherent destination file");

    int i = 0;
    foreach (StringItem si, *list) {
        if (this->tranStrings.at(i).getIndex() != si.getIndex())
            throw std::length_error("Incoherent destination file");
        else {
            this->tranStrings.replace(i, si);
            i++;
        }
    }
}

StringList * TRAHandler::extractStringItemsFromFile(QString textFileName) {
    StringList *ans = new StringList();

    QString wholeText, line;
    QStringList stringList, stringItemslist;
    unsigned int stringIndex;
    QFile textFile(textFileName);
    QString string;
    QStringList::iterator iter;
    bool stat;

    if (!textFile.open(QIODevice::ReadOnly | QIODevice::Text))
        throw std::runtime_error("Could not open file");

    QTextStream in(&textFile);
    line = in.readLine();
    while (!line.isNull()) {
        wholeText.append(line);
        if (!line.endsWith("~"))
            wholeText.append("\n");
        line = in.readLine();
    }

    stringList = wholeText.split('@', QString::SkipEmptyParts);

    for (iter = stringList.begin(); iter != stringList.end(); ++iter) {
        stringItemslist = iter->split(QRegExp("=+~"), QString::SkipEmptyParts);

        if (stringItemslist.size() != 2)
            throw std::invalid_argument("Error on parsing input file: invalid token ID/string");


        stringIndex = stringItemslist.at(0).toUInt(&stat, 10);

        if (!stat)
            throw std::invalid_argument("Error on parsing input file: invalid index");

        string = stringItemslist.at(1);

        //string.remove(QRegExp("[^a-zA-Z\\d\\s]"));
        //string.remove(QRegExp("[~\\n$]"));
        string.remove(QRegExp("~\\n?"));

        ans->append(*(new StringItem(stringIndex, string)));
    }

    textFile.close();

    return ans;
}

void TRAHandler::setDestFilePath(const QString &value)
{
    destFilePath = value;
}

QString TRAHandler::getDestFilePath() const
{
    return destFilePath;
}

TRAHandler::TRAHandler(){}

TRAHandler::TRAHandler(QString filepath) {
    this->sourceFilePath = filepath;
}

void TRAHandler::init() {
    StringList *list = extractStringItemsFromFile(this->sourceFilePath);

    this->origStrings = *list;
    foreach (StringItem si, this->origStrings) {
        this->tranStrings.append(*(new StringItem(si.getIndex(), *(new QString()))));
    }
}

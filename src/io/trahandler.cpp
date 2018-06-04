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

TRAHandler::TRAHandler(){}

TRAHandler::TRAHandler(QString filepath) {
    this->filePath = filepath;
}

void TRAHandler::init() {
    QString wholeText, line;
    QFile textFile(this->filePath);
    QStringList stringList, stringItemslist;
    unsigned int stringIndex;
    QString string;
    QStringList::iterator iter;
    bool stat;

    if (!textFile.open(QIODevice::ReadOnly | QIODevice::Text))
        throw std::runtime_error("Could not open file");

    QTextStream in(&textFile);
    line = in.readLine();
    while (!line.isNull()) {
        wholeText.append(line+"\n");
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

        string.remove('~');

        this->origStrings.append(*(new StringItem(stringIndex, string)));
        this->tranStrings.append(*(new StringItem(stringIndex, *(new QString()))));
    }
}

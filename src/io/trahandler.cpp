#include "tra.hpp"

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
        wholeText.append(line);
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

        this->strings.append(*(new StringItem(stringIndex, string)));
    }
}

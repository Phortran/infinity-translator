#ifndef TRA_HPP
#define TRA_HPP

#include <QString>
#include <QLinkedList>
#include <QFile>
#include <QTextStream>
#include "stringitem.hpp"
#include "utils.hpp"

class TRAHandler
{
private:
    QString sourceFilePath;
    QString destFilePath;
    StringList origStrings;
    StringList tranStrings;

    TRAHandler();
    StringList * extractStringItemsFromFile(QString textFileName);

public:
    static const bool ORIG = true;
    static const bool TRAN = false;

    TRAHandler(QString filepath);
    void init();
    StringList getOrigStrings() const;
    StringItem getStringItemAt(int index, bool which);
    StringList getTranStrings() const;
    void setTranStrings();
    void setStringAt(int index, QString data);
    void setDestFilePath(const QString &value);
    QString getDestFilePath() const;
};

#endif // TRA_HPP

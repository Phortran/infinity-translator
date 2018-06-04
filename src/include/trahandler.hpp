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
    QString filePath;
    StringList origStrings;
    StringList tranStrings;

    TRAHandler();

public:
    static const bool ORIG = true;
    static const bool TRAN = false;

    TRAHandler(QString filepath);
    void init();
    StringList getOrigStrings() const;
    StringItem getStringItemAt(int index, bool which);
    StringList getTranStrings() const;
    void setStringAt(int index, QString data);
};

#endif // TRA_HPP

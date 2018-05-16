#ifndef TRA_HPP
#define TRA_HPP

#include <QString>
#include <QLinkedList>
#include <QFile>
#include <QTextStream>
#include "stringitem.hpp"

class TRAHandler
{
private:
    QString filePath;
    QLinkedList<StringItem> strings;

    TRAHandler();

public:
    TRAHandler(QString filepath);
    void init();
};

#endif // TRA_HPP

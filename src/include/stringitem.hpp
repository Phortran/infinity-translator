#ifndef STRINGITEM_H
#define STRINGITEM_H

#include<QString>

class StringItem {
public:
    StringItem(unsigned int index, QString text);
    ~StringItem();

    unsigned int getIndex() const;
    void setIndex(const unsigned int &value);

    QString getText() const;
    void setText(const QString &value);

    QString getTextIndex() const;

private:
    //    QGridLayout *stringItemLayout;
    //    QPushButton *button1;
//    QPushButton *button2;
    unsigned int index;
    QString text;

    StringItem();
};

typedef QList<StringItem> StringList;

#endif // STRINGITEM_H

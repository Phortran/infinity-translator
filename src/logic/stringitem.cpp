#include "stringitem.hpp"

StringItem::StringItem() {
//    this->stringItemLayout = new QGridLayout;
//    this->button1 = new QPushButton;
//    this->button2 = new QPushButton;

//    this->stringItemLayout->addWidget(this->button1, 0, 0);
//    this->stringItemLayout->addWidget(this->button2, 0, 1);
//    this->setLayout(this->stringItemLayout);
}

StringItem::StringItem(unsigned int index, QString text) {
    this->index = index;
    this->text = text;
}

StringItem::~StringItem() {
//    delete this->stringItemLayout;
//    delete this->button1;
//    delete this->button2;
}

unsigned int StringItem::getIndex() const
{
    return index;
}

void StringItem::setIndex(const unsigned int &value)
{
    index = value;
}

QString StringItem::getText() const
{
    return text;
}

void StringItem::setText(const QString &value)
{
    text = value;
}

QString StringItem::getTextIndex() const
{
    return QString::number(this->index);
}

#include "stringitem.h"

StringItem::StringItem(QWidget *parent) : QGroupBox(parent)
{
    this->stringItemLayout = new QGridLayout;
    this->button1 = new QPushButton;
    this->button2 = new QPushButton;

    this->stringItemLayout->addWidget(this->button1, 0, 0);
    this->stringItemLayout->addWidget(this->button2, 0, 1);
    this->setLayout(this->stringItemLayout);
}

StringItem::~StringItem()
{
    delete this->stringItemLayout;
    delete this->button1;
    delete this->button2;
}

#include "stringitem.h"

StringItem::StringItem(QWidget *parent) : QWidget(parent)
{
    this->baseBox = new QGroupBox;
    this->stringItemLayout = new QGridLayout;
    this->button1 = new QPushButton;
    this->button2 = new QPushButton;

    this->baseBox->setLayout(this->stringItemLayout);
    this->stringItemLayout->addWidget(this->button1, 0, 0);
    this->stringItemLayout->addWidget(this->button2, 0, 1);
}

StringItem::~StringItem()
{
    delete this->baseBox;
    delete this->stringItemLayout;
    delete this->button1;
    delete this->button2;
}

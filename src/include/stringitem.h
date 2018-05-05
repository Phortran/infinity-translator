#ifndef STRINGITEM_H
#define STRINGITEM_H

#include <QWidget>
#include <QtWidgets>

class StringItem : public QWidget
{
    Q_OBJECT
public:
    explicit StringItem(QWidget *parent = nullptr);
    ~StringItem();

private:
    QGroupBox *baseBox;
    QGridLayout *stringItemLayout;
    QPushButton *button1;
    QPushButton *button2;

signals:

public slots:
};

#endif // STRINGITEM_H

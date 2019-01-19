#include "inputform.h"

#include <QValidator>
#include <QDebug>
#include <QString>
#include <QWidget>

InputForm::InputForm(QWidget *widget)
{
    rowInput = new QLineEdit(widget);
    rowInput->setPlaceholderText("Row");
    rowInput->setValidator(new QIntValidator(1, 15));
    colInput = new QLineEdit(widget);
    colInput->setPlaceholderText("Column");
    colInput->setValidator(new QIntValidator(1, 15));
}

InputForm::~InputForm()
{
    delete rowInput;
    delete colInput;
}

// 设置地图信号
void InputForm::setMapSize()
{
    QString r = rowInput->text();
    QString c = colInput->text();
//    qDebug() << r << ' ' << c;
    emit sentMapSize(r.toInt(), c.toInt());
}

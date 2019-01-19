#ifndef INPUTFORM_H
#define INPUTFORM_H

#include <QLineEdit>
#include <QWidget>

class InputForm : public QObject
{
    Q_OBJECT
public:
    explicit InputForm(QWidget *widget);
    ~InputForm();
    void setMapSize();

    QLineEdit *rowInput;
    QLineEdit *colInput;

signals:
    void sentMapSize(int, int);
};

#endif // INPUTFORM_H

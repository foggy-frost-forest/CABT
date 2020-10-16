#ifndef NODEBUTTON_H
#define NODEBUTTON_H

#include <QPushButton>
#include <QWidget>
#include <QTimer>
#include <QLabel>
#include "infowidget.h"

class NodeButton : public QPushButton
{
    Q_OBJECT
public:
    explicit NodeButton(QWidget *parent = nullptr);
    int *x,*y;
    void init(int num,int sx,int sy);
    void setfa(QWidget *fa) {father=fa;}
    void mymove(int ex,int ey,int fps=60,int t=1000);
    void change(int d1=-1,int d2=-1,int d3=-1,int d4=-1,int d5=-1,int d6=-1);
    void point();
    void inpoint();
    void title(int x);
    int *data_1,*data_2,*data_3,*data_4,*data_5,*data_6;
    QWidget *father;
    QLabel *lab_1,*lab_2,*lab_3;
    InfoWidget *info;
    QTimer *clock;
signals:

};

#endif // NODEBUTTON_H

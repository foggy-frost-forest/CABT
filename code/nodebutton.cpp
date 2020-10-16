#include "nodebutton.h"
#include <QDebug>

NodeButton::NodeButton(QWidget *parent) : QPushButton(parent)
{
    clock = new QTimer(this);
    x = new int(0);
    y = new int(0);
    data_1 = new int(-1);
    data_2 = new int(-1);
    data_3 = new int(-1);
    data_4 = new int(-1);
    data_5 = new int(-1);
    data_6 = new int(-1);
    info = new InfoWidget;
    lab_1 = new QLabel(this);
    lab_2 = new QLabel(this);
    lab_3 = new QLabel(this);
    lab_1->resize(40,20);
    lab_1->move(5,5);
    lab_2->resize(20,20);
    lab_2->move(5,25);
    lab_3->resize(20,20);
    lab_3->move(25,25);
    lab_1->setAlignment(Qt::AlignHCenter);
    lab_2->setAlignment(Qt::AlignHCenter);
    lab_3->setAlignment(Qt::AlignHCenter);
    this->setText("");
    this->inpoint();
    connect(this,&QPushButton::clicked,[=](){
        info->show();
    });
}
void NodeButton::init(int num,int sx, int sy)
{
    *x=sx;
    *y=sy;
    info->title=num;
    this->move(*x,*y);
    this->inpoint();
}
void NodeButton::title(int num)
{
    info->title=num;
}
void NodeButton::point()
{
    this->setStyleSheet("background-color:rgba(0,255,255,255);border:4px solid rgba(0,0,255,255);border-radius:10px;");
    lab_1->setStyleSheet("background-color:rgba(0,255,255,255);border:0px;");
    lab_2->setStyleSheet("background-color:rgba(0,255,255,255);border:0px;");
    lab_3->setStyleSheet("background-color:rgba(0,255,255,255);border:0px;");

}
void NodeButton::inpoint()
{
    this->setStyleSheet("background-color:rgba(0,255,255,255);border:0px solid rgba(0,255,255,255);border-radius:10px;");
    lab_1->setStyleSheet("background-color:rgba(0,255,255,255);border:0px;");
    lab_2->setStyleSheet("background-color:rgba(0,255,255,255);border:0px;");
    lab_3->setStyleSheet("background-color:rgba(0,255,255,255);border:0px;");
}
void NodeButton::mymove(int ex,int ey,int fps,int t)
{
//    QTimer *clock = new QTimer(this);
    //qDebug() << 0 << *x << *y << ex << ey;
    int ts,dx,dy,px,py,cnt=1,fx=1,fy=1,cx=*x,cy=*y,per;
    if(*x>ex) cx=2*ex-*x,fx=-1;
    if(*y>ey) cy=2*ey-*y,fy=-1;
    ts=t*fps/1000;
    dx=(ex-cx)/ts;
    dy=(ey-cy)/ts;
    px=ex-cx-dx*ts;
    py=ey-cy-dy*ts;
    per=1000/fps;
    if(cnt<=px) dx++;
    if(cnt<=py) dy++;
    clock->start(per);
    connect(clock,&QTimer::timeout,[=]()mutable{
        if(cnt<=ts)
        {
            *x+=dx*fx;
            *y+=dy*fy;
            this->move(*x,*y);
            if(cnt==px) dx--;
            if(cnt==py) dy--;
            cnt++;
            father->update();
        }
        else
        {
            clock->stop();
            disconnect(clock,&QTimer::timeout,0,0);
        }
    });
}
void NodeButton::change(int d1, int d2, int d3, int d4, int d5,int d6)
{
    if(d1!=-2) *data_1=d1;
    if(d2!=-2) *data_2=d2;
    if(d3!=-2) *data_3=d3;
    if(d4!=-2) *data_4=d4;
    if(d5!=-2) *data_5=d5;
    if(d6!=-2) *data_6=d6;
    this->lab_1->setText(*data_1>100000 ? (QString)('a'+*data_1-100001) : QString::number(*data_1));
    if(d2!=-1 && d2!=-2) this->lab_2->setText(*data_2==-1 ? "" : QString::number(*data_2));
    else if(d2==-1 && d5!=-2) this->lab_2->setText(*data_5==-1 ? "" : QString::number(*data_5));
    if(d3!=-1 && d3!=-2) this->lab_3->setText(*data_3==-1 ? "" : QString::number(*data_3));
    else if(d3==-1 && d6!=-2) this->lab_3->setText(*data_6==-1 ? "" : QString::number(*data_6));
    info->change(d1,d2,d3,d4,d5,d6);
}

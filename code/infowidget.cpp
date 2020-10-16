#include "infowidget.h"
#include "ui_infowidget.h"

InfoWidget::InfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InfoWidget)
{
    ui->setupUi(this);
    //this->setWindowFlags(Qt::FramelessWindowHint);
    this->setFixedSize(300,200);
}
void InfoWidget::closeEvent(QCloseEvent *event)
{
    this->hide();
    event->ignore();;
}
void InfoWidget::change(int d1, int d2, int d3, int d4, int d5, int d6)
{
    if(d1!=-2)
    {
        if(d1<=100000)
            ui->lab_1->setText("Node:"+QString::number(title)+"  |  Value:"+QString::number(d1));
        else
            ui->lab_1->setText("Node:"+QString::number(title)+"  |  Value:"+(QString)('a'+d1-100001));
    }
    if(d2!=-2)
    {
        if(d2!=-1) ui->lab_2->setText("left:"+QString::number(d2));
        else ui->lab_2->setText("");
    }
    if(d3!=-2)
    {
        if(d3!=-1) ui->lab_3->setText("Right:"+QString::number(d3));
        else ui->lab_3->setText("");
    }
    if(d4!=-2)
    {
        if(d4!=-1) ui->lab_4->setText("Father:"+QString::number(d4));
        else ui->lab_4->setText("Root");
    }
    if(d5!=-2)
    {
        if(d5!=-1) ui->lab_5->setText("Left Son:"+QString::number(d5));
        else ui->lab_5->setText("Left Son:NULL");
    }
    if(d6!=-2)
    {
        if(d6!=-1) ui->lab_6->setText("Right Son:"+QString::number(d6));
        else ui->lab_6->setText("Right Son:NULL");
    }
}
QLabel * InfoWidget::get_lab(int x)
{
    if(x==1) return ui->lab_1;
    if(x==2) return ui->lab_2;
    if(x==3) return ui->lab_3;
    if(x==4) return ui->lab_4;
    if(x==5) return ui->lab_5;
    return ui->lab_6;
}
InfoWidget::~InfoWidget()
{
    delete ui;
}

#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);


    ui->stackedWidget->setCurrentIndex(0);
    connect(ui->start_pb,&QPushButton::clicked,this,[=]()
    {
       ui->stackedWidget->setCurrentIndex(1);
    });
    connect(ui->back_p1_pb,&QPushButton::clicked,this,[=]()
    {
       ui->stackedWidget->setCurrentIndex(0);
    });
    connect(ui->back_p2_pb,&QPushButton::clicked,this,[=]()
    {
       ui->stackedWidget->setCurrentIndex(1);
    });


    connect(ui->pb1,&QPushButton::clicked,this,[=]()
    {
        flag=1;
        ui->stackedWidget->setCurrentIndex(2);
        judge();
    });
    connect(ui->pb2,&QPushButton::clicked,this,[=]()
    {
        flag=2;
        ui->stackedWidget->setCurrentIndex(2);
        judge();
    });
    connect(ui->pb3,&QPushButton::clicked,this,[=]()
    {
        flag=3;
        ui->stackedWidget->setCurrentIndex(2);
        judge();
    });
    connect(ui->pb4,&QPushButton::clicked,this,[=]()
    {
        flag=4;
        ui->stackedWidget->setCurrentIndex(2);
        judge();
    });
    connect(ui->pb5,&QPushButton::clicked,this,[=]()
    {
        flag=5;
        ui->stackedWidget->setCurrentIndex(2);
        judge();
    });
    connect(ui->pb6,&QPushButton::clicked,this,[=]()
    {
        flag=6;
        ui->stackedWidget->setCurrentIndex(2);
        judge();
    });
    /*
    connect(ui->pb7,&QPushButton::clicked,this,[=]()
    {
        flag=7;
        ui->stackedWidget->setCurrentIndex(2);
        judge();
    });
    connect(ui->pb8,&QPushButton::clicked,this,[=]()
    {
        flag=8;
        ui->stackedWidget->setCurrentIndex(2);
        judge();
    });
    connect(ui->pb9,&QPushButton::clicked,this,[=]()
    {
        flag=9;
        ui->stackedWidget->setCurrentIndex(2);
        judge();
    });
    connect(ui->pb10,&QPushButton::clicked,this,[=]()
    {
        flag=10;
        ui->stackedWidget->setCurrentIndex(2);
        judge();
    });
    connect(ui->pb11,&QPushButton::clicked,this,[=]()
    {
        flag=11;
        ui->stackedWidget->setCurrentIndex(2);
        judge();
    });
    connect(ui->pb12,&QPushButton::clicked,this,[=]()
    {
        flag=12;
        ui->stackedWidget->setCurrentIndex(2);
        judge();
    });
    */


    connect(ui->example_pb,&QPushButton::clicked,this,[=]()
    {
        QFile file(example);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QByteArray tmp = file.readAll();
        ui->input_text->setPlainText(QString(tmp));
        file.close();
    });
    connect(ui->do_pb,&QPushButton::clicked,this,[=]()
    {
        QString input = ui->input_text->toPlainText();
        QFile file("./data/intxt.txt");
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        file.write(input.toUtf8());
        file.close();
        switch(flag)
        {
        case 1:
            bst_son = new BST;
            bst_son->show();
            break;
        case 2:
            avl_son = new AVL;
            avl_son->show();
            break;
        case 3:
            heap_son = new HEAP;
            heap_son->show();
            break;
        case 4:
            sgt_son = new SGT;
            sgt_son->show();
            break;
        case 5:
            pst_son = new PST;
            pst_son->show();
            break;
        case 6:
            ackmp_son = new ACKMP;
            ackmp_son->show();
            break;
        }
    });
}
void Widget::judge()
{
    ui->input_text->clear();
    switch(flag)
    {
    case 1:
        example="./data/example1.txt";
        describe="./data/describe1.txt";
        break;
    case 2:
        example="./data/example2.txt";
        describe="./data/describe2.txt";
        break;
    case 3:
        example="./data/example3.txt";
        describe="./data/describe3.txt";
        break;
    case 4:
        example="./data/example4.txt";
        describe="./data/describe4.txt";
        break;
    case 5:
        example="./data/example5.txt";
        describe="./data/describe5.txt";
        break;
    case 6:
        example="./data/example6.txt";
        describe="./data/describe6.txt";
        break;
    /*
    case 7:
        example="example7.txt";
        describe="describe7.txt";
        break;
    case 8:
        example="example8.txt";
        describe="describe8.txt";
        break;
    */
    }
    QFile file1(example),file2(describe);
    file1.open(QIODevice::ReadOnly | QIODevice::Text);
    file2.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray tmp = file1.readAll();
    ui->example_lab->setText(QString(tmp));
    tmp = file2.readAll();
    ui->describe_lab->setText(QString(tmp));
    file1.close();
    file2.close();
}

Widget::~Widget()
{
    delete ui;
}


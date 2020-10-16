#ifndef ACKMP_H
#define ACKMP_H

#include <QMainWindow>
#include <QStatusBar>
#include <QToolBar>
#include <QMenuBar>
#include <QDockWidget>
#include <QTextEdit>
#include <QTimer>
#include <QTime>
#include <QPainter>
#include <QPaintEvent>
#include <QPushButton>
#include <QThread>
#include <QLayout>
#include <QCoreApplication>
#include <queue>
#include <iostream>
#include <vector>
#include "nodebutton.h"
#include "orderlist.hpp"

#define myMAX 200

struct AC
{
    char data;
    int ac;
    int son[3];
    int tag;
};
using namespace std;

namespace Ui {
class ACKMP;
}

class ACKMP : public QMainWindow
{
    Q_OBJECT

public:
    explicit ACKMP(QWidget *parent = nullptr);
    ~ACKMP();

private:
    Ui::ACKMP *ui;
    NodeButton *pb[50];
    QLabel *lab[20],*output_lab,*status_lab;
    QMenuBar *menubar;
    QMenu *debug_menu,*windows_menu;
    QToolBar *toolbar;
    QAction *run_ac,*next_ac,*pause_ac,*back_ac,*restart_ac;
    QAction *run_mac,*next_mac,*pause_mac,*back_mac,*restart_mac;
    QAction *toolshow_mac,*toolhide_mac,*dockshow_mac,*dockhide_mac;
    QTimer * clock;
    QDockWidget *dock_widget;
    OrderList ord;
    QWidget *show_widget,*code_widget;
    vector <pair<int,int>> drawlist;
    enum MODE {pause_mode,run_mode,next_mode,start_mode,back_mode} mode;
    int lab_top;
    unsigned int now;
    void mysleep(int msec);
    bool eventFilter(QObject *watched, QEvent *event);

    int treenodex[50]={700,300,700,1100,200,300,400,600,700,800,
                        1000,1100,1200};
    int treenodey[50]={100,400,400,400,700,700,700,700,700,700,
                       700,700,700};
    AC tree[myMAX];
    int n,ans[10];
    char txt[100],key[10][5];
    void deal();
    void init(int x,char y);
    void creat();
    void bfs();
    void judge();
public slots:
    void turn_next_mode();
    void turn_run_mode();
    void turn_pause_mode();
    void turn_start_mode();
    void turn_back_mode();
    void toolbar_show(){toolbar->show();}
    void toolbar_hide(){toolbar->hide();}
    void dock_show(){dock_widget->show();}
    void dock_hide(){dock_widget->hide();}
};

#endif // ACKMP_H

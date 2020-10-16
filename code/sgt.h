#ifndef SGT_H
#define SGT_H

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
#include <iostream>
#include <vector>
#include "nodebutton.h"
#include "orderlist.hpp"

#define myMAX 200
using namespace std;

namespace Ui {
class SGT;
}

class SGT : public QMainWindow
{
    Q_OBJECT

public:
    explicit SGT(QWidget *parent = nullptr);
    ~SGT();

private:
    Ui::SGT *ui;
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

    int n,m,num[myMAX],lazy[myMAX];
    void deal();
    void add(int l,int r,int p,int s,int e,int k);
    int find(int l,int r,int p,int s,int e);
    void creat(int l,int r,int p);
    int treenodex[50]={0,630,310,950,150,470,790,1110,
                       70,230,390,550,710,870,1030,1190,
                       30,110,190,270,350,430,510,590,670,
                       750,830,910,990,1070,1150,1230};
    int treenodey[50]={0,60,220,220,380,380,380,380,540,
                       540,540,540,540,540,540,540,700,
                       700,700,700,700,700,700,700,700,
                       700,700,700,700,700,700,700};

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

#endif // SGT_H

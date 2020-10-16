#ifndef AVL_H
#define AVL_H

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
class AVL;
}

class AVL : public QMainWindow
{
    Q_OBJECT
public:
    explicit AVL(QWidget *parent = nullptr);
    ~AVL();
private:
    Ui::AVL *ui;
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

    int n,top,val[myMAX],key[myMAX],st[myMAX];
    int tree[myMAX],ld[myMAX],rd[myMAX],f[myMAX];
    int size[myMAX],bla[myMAX],son[myMAX][2];
    int treenodex[50]={500,630,310,950,150,470,790,1110,
                       70,230,390,550,710,870,1030,1190,
                       30,110,190,270,350,430,510,590,670,
                       750,830,910,990,1070,1150,1230};
    int treenodey[50]={30,60,220,220,380,380,380,380,540,
                       540,540,540,540,540,540,540,700,
                       700,700,700,700,700,700,700,700,
                       700,700,700,700,700,700,700};
    void deal();
    void rotate(int x,int y);
    void myupdate(int x);
    void add(int x,int p);
    void up(int x);
    void mysleep(int msec);
    bool eventFilter(QObject *watched, QEvent *event);
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

#endif // AVL_H

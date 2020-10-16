#ifndef PST_H
#define PST_H

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

struct TREE
{
    int w,lef,rig;
    int lson,rson;
};
using namespace std;

namespace Ui {
class PST;
}
class PST : public QMainWindow
{
    Q_OBJECT
public:
    explicit PST(QWidget *parent = nullptr);
    ~PST();
private:
    Ui::PST *ui;
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

    int treenodex[50]={0,275,125,425,50,200,350,500,650,650,650,
                        800,800,800,950,950,950,1100,1100,1100,
                        1250,1250,1250,1400,1400,1400};
    int treenodey[50]={0,100,350,350,600,600,600,600,100,350,600,
                       100,350,600,100,350,600,100,350,600,100,
                       350,600,100,350,600};

    int n,m,top,ans;
    TREE tree[myMAX];
    int a[myMAX],head[myMAX];
    void creat(int l,int r,int x);
    void insert(int l,int r,int x,int p);
    int ask(int x,int y,int z);
    void deal();
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
#endif // PST_H

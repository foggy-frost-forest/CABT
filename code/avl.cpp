#include "avl.h"
#include "ui_avl.h"
#include <QDebug>

AVL::AVL(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AVL)
{
    ui->setupUi(this);
    this->setFixedSize(1600,900);
    this->setWindowTitle("running");
    this->setContextMenuPolicy(Qt::NoContextMenu);

    deal();
    clock = new QTimer(this);
    show_widget = ui->showwidget;
    code_widget = ui->codewidget;
    dock_widget = ui->dockwidget;
    output_lab = ui->outputlab;
    status_lab = ui->statuslab;
    menubar = menuBar();
    toolbar = new QToolBar(this);

    show_widget->installEventFilter(this);
    addDockWidget(Qt::RightDockWidgetArea,dock_widget);
    dock_widget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    output_lab->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    status_lab->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    setMenuBar(menubar);
    debug_menu = menubar->addMenu("Debug");
    run_mac = debug_menu->addAction("Run");
    next_mac = debug_menu->addAction("Next");
    pause_mac = debug_menu->addAction("Pause");
    back_mac = debug_menu->addAction("Back");
    restart_mac = debug_menu->addAction("Restart");
    windows_menu = menubar->addMenu("Windows");
    toolshow_mac = windows_menu->addAction("tool show");
    toolhide_mac = windows_menu->addAction("tool hide");
    dockshow_mac = windows_menu->addAction("dock show");
    dockhide_mac = windows_menu->addAction("dock hide");

    addToolBar(Qt::TopToolBarArea,toolbar);
    toolbar->setFloatable(false);
    toolbar->setAllowedAreas(Qt::LeftToolBarArea | Qt::RightToolBarArea | Qt::TopToolBarArea);
    run_ac = toolbar->addAction("Run");
    next_ac = toolbar->addAction("Next");
    pause_ac = toolbar->addAction("Pause");
    back_ac = toolbar->addAction("Back");
    restart_ac = toolbar->addAction("Restart");

    connect(run_ac,SIGNAL(triggered()),this,SLOT(turn_run_mode()));
    connect(pause_ac,SIGNAL(triggered()),this,SLOT(turn_pause_mode()));
    connect(next_ac,SIGNAL(triggered()),this,SLOT(turn_next_mode()));
    connect(back_ac,SIGNAL(triggered()),this,SLOT(turn_back_mode()));
    connect(restart_ac,SIGNAL(triggered()),this,SLOT(turn_start_mode()));
    connect(run_mac,SIGNAL(triggered()),this,SLOT(turn_run_mode()));
    connect(pause_mac,SIGNAL(triggered()),this,SLOT(turn_pause_mode()));
    connect(next_mac,SIGNAL(triggered()),this,SLOT(turn_next_mode()));
    connect(back_mac,SIGNAL(triggered()),this,SLOT(turn_back_mode()));
    connect(restart_mac,SIGNAL(triggered()),this,SLOT(turn_start_mode()));
    connect(toolshow_mac,SIGNAL(triggered()),this,SLOT(toolbar_show()));
    connect(toolhide_mac,SIGNAL(triggered()),this,SLOT(toolbar_hide()));
    connect(dockshow_mac,SIGNAL(triggered()),this,SLOT(dock_show()));
    connect(dockhide_mac,SIGNAL(triggered()),this,SLOT(dock_hide()));

    for(int i=0;i<=31;i++)
    {
        pb[i]=new NodeButton;
        pb[i]->setParent(show_widget);
        pb[i]->resize(50,50);
        pb[i]->init(i,treenodex[i],treenodey[i]);
        pb[i]->hide();
        pb[i]->setfa(this);
    }
    QVBoxLayout *hlay = new QVBoxLayout();
    lab_top=3;
    for(int i=1;i<=lab_top;i++)
    {
        lab[i]=new QLabel;
        lab[i]->setWordWrap(true);
        hlay->addWidget(lab[i]);
    }
    code_widget->setLayout(hlay);
    lab[1]->setText("1.add node");
    lab[2]->setText("2.update (x) balance tree_size left_son_depth right_son_depth");
    lab[3]->setText("3.rotate");

    now=0;
    ord.point_init();
    connect(clock,&QTimer::timeout,[=]()mutable{
        if(now<ord.list.size())
        {
            if(ord.list[now]!=ord.break_point)
            {
                if(ord.list[now]==ord.node_add)
                {
                    int a=ord.l_nodeadd[ord.na_p].num;
                    *(pb[a]->x)=treenodex[st[a]];
                    *(pb[a]->y)=treenodey[st[a]];
                    pb[a]->move(treenodex[st[a]],treenodey[st[a]]);
                    pb[a]->show();
                    ord.na_p++;
                }
                else if(ord.list[now]==ord.node_cge)
                {
                    NodeCge tmp=ord.l_nodecge[ord.nc_p];
                    pb[tmp.num]->change(tmp.data_1,tmp.data_2,tmp.data_3,tmp.data_4,tmp.data_5,tmp.data_6);
                    ord.nc_p++;
                }
                else if(ord.list[now]==ord.node_poi)
                {
                    pb[ord.l_nodepoi[ord.np_p].num]->point();
                    ord.np_p++;
                }
                else if(ord.list[now]==ord.node_inpoi)
                {
                    pb[ord.l_nodeinpoi[ord.nip_p].num]->inpoint();
                    ord.nip_p++;
                }
                else if(ord.list[now]==ord.line_add)
                {
                    int a=ord.l_lineadd[ord.la_p].num_fst;
                    int b=ord.l_lineadd[ord.la_p].num_sec;
                    drawlist.push_back(make_pair(a,b));
                    update();
                    ord.la_p++;
                }
                else if(ord.list[now]==ord.line_cge)
                {
                    int a=ord.l_linecge[ord.lc_p].old_fst;
                    int b=ord.l_linecge[ord.lc_p].old_sec;
                    int c=ord.l_linecge[ord.lc_p].new_fst;
                    int d=ord.l_linecge[ord.lc_p].new_sec;
                    for(unsigned int i=0;i<drawlist.size();i++)
                    {
                        if(drawlist[i].first==a && drawlist[i].second==b)
                        {
                            drawlist[i].first=c;
                            drawlist[i].second=d;
                            break;
                        }
                        if(drawlist[i].first==b && drawlist[i].second==a)
                        {
                            drawlist[i].first=c;
                            drawlist[i].second=d;
                            break;
                        }
                    }
                    update();
                    ord.lc_p++;
                }
                else if(ord.list[now]==ord.node_move)
                {
                    int a=ord.l_nodemove[ord.nm_p].num_to;
                    update();
                    pb[ord.l_nodemove[ord.nm_p].num_from]->mymove(treenodex[a],treenodey[a]);
                    ord.nm_p++;
                    update();
                }
                else if(ord.list[now]==ord.code_show)
                {
                    for(int i=1;i<=lab_top;i++)
                    {
                        if(i==ord.l_codeshow[ord.cs_p].num)
                            lab[i]->setStyleSheet("background-color:rgba(255,0,0,100);");
                        else
                            lab[i]->setStyleSheet("background-color:rgba(0,0,0,0);");
                    }
                    ord.cs_p++;
                }
                now++;
            }
            else
            {
                if(mode==run_mode)
                {
                    mysleep(1000);
                    now++;
                }
                else if(mode==pause_mode)
                {
                    clock->stop();
                }
                else if(mode==next_mode)
                {
                    now++;
                    mode=pause_mode;
                }
                else if(mode==back_mode)
                {
                    clock->stop();
                    for(int i=0;i<=31;i++)
                    {
                        pb[i]->hide();
                        pb[i]->change(0);
                        pb[i]->init(i,treenodex[i],treenodey[i]);
                    }
                    for(int i=1;i<=lab_top;i++)
                        lab[i]->setStyleSheet("background-color:rgba(0,0,0,0);");
                    output_lab->setText("");
                    drawlist.clear();
                    ord.point_init();
                    unsigned int tmpnow;
                    for(unsigned int i=0,j=0;i<=now;i++)
                        if(ord.list[i]==ord.break_point)
                        {
                            tmpnow=j;
                            j=i;
                        }
                    now=0;
                    while(now<tmpnow)
                    {
                        if(ord.list[now]==ord.node_add)
                        {
                            int a=ord.l_nodeadd[ord.na_p].num;
                            *(pb[a]->x)=treenodex[st[a]];
                            *(pb[a]->y)=treenodey[st[a]];
                            pb[a]->move(treenodex[st[a]],treenodey[st[a]]);
                            pb[a]->show();
                            ord.na_p++;
                        }
                        else if(ord.list[now]==ord.node_cge)
                        {
                            NodeCge tmp=ord.l_nodecge[ord.nc_p];
                            pb[tmp.num]->change(tmp.data_1,tmp.data_2,tmp.data_3,tmp.data_4,tmp.data_5,tmp.data_6);
                            ord.nc_p++;
                        }
                        else if(ord.list[now]==ord.node_poi)
                        {
                            pb[ord.l_nodepoi[ord.np_p].num]->point();
                            ord.np_p++;
                        }
                        else if(ord.list[now]==ord.node_inpoi)
                        {
                            pb[ord.l_nodeinpoi[ord.nip_p].num]->inpoint();
                            ord.nip_p++;
                        }
                        else if(ord.list[now]==ord.line_add)
                        {
                            int a=ord.l_lineadd[ord.la_p].num_fst;
                            int b=ord.l_lineadd[ord.la_p].num_sec;
                            drawlist.push_back(make_pair(a,b));
                            ord.la_p++;
                        }
                        else if(ord.list[now]==ord.line_cge)
                        {
                            int a=ord.l_linecge[ord.lc_p].old_fst;
                            int b=ord.l_linecge[ord.lc_p].old_sec;
                            int c=ord.l_linecge[ord.lc_p].new_fst;
                            int d=ord.l_linecge[ord.lc_p].new_sec;
                            for(unsigned int i=0;i<drawlist.size();i++)
                            {
                                if(drawlist[i].first==a && drawlist[i].second==b)
                                {
                                    drawlist[i].first=c;
                                    drawlist[i].second=d;
                                    break;
                                }
                                if(drawlist[i].first==b && drawlist[i].second==a)
                                {
                                    drawlist[i].first=c;
                                    drawlist[i].second=d;
                                    break;
                                }
                            }
                            ord.lc_p++;
                        }
                        else if(ord.list[now]==ord.node_move)
                        {
                            int a=ord.l_nodemove[ord.nm_p].num_to;
                            pb[ord.l_nodemove[ord.nm_p].num_from]->move(treenodex[a],treenodey[a]);
                            *(pb[ord.l_nodemove[ord.nm_p].num_from]->x)=treenodex[a];
                            *(pb[ord.l_nodemove[ord.nm_p].num_from]->y)=treenodey[a];
                            ord.nm_p++;
                        }
                        else if(ord.list[now]==ord.code_show)
                        {
                            for(int i=1;i<=lab_top;i++)
                            {
                                if(i==ord.l_codeshow[ord.cs_p].num)
                                    lab[i]->setStyleSheet("background-color:rgba(255,0,0,100);");
                                else
                                    lab[i]->setStyleSheet("background-color:rgba(0,0,0,0);");
                            }
                            ord.cs_p++;
                        }
                        update();
                        now++;
                    }
                    mode=pause_mode;
                }
                else if(mode==start_mode)
                {
                    clock->stop();
                    for(int i=0;i<=31;i++)
                    {
                        pb[i]->hide();
                        pb[i]->change(0);
                        pb[i]->init(i,treenodex[i],treenodey[i]);
                    }
                    for(int i=1;i<=lab_top;i++)
                        lab[i]->setStyleSheet("background-color:rgba(0,0,0,0);");
                    output_lab->setText("");
                    drawlist.clear();
                    update();
                    now=0;
                    ord.point_init();
                    mode=pause_mode;
                }
            }
        }
    });
}
void AVL::turn_next_mode()
{
    mode=next_mode;
    status_lab->setText("step");
    clock->start(10);
}
void AVL::turn_run_mode()
{
    mode=run_mode;
    status_lab->setText("running");
    clock->start(10);
}
void AVL::turn_pause_mode()
{
    mode=pause_mode;
    status_lab->setText("pause");
}
void AVL::turn_start_mode()
{
    mode=start_mode;
    if(now==ord.list.size()) now--;
    status_lab->setText("pause");
    clock->start(10);
}
void AVL::turn_back_mode()
{
    mode=back_mode;
    if(now==ord.list.size()) now--;
    status_lab->setText("pause");
    clock->start(10);
}
void AVL::mysleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
bool AVL::eventFilter(QObject *obj, QEvent *e)
{
    if(obj == ui->showwidget && e->type() == QEvent::Paint)
    {
        QPainter painter(ui->showwidget);
        for(unsigned int i=0,a,b;i<drawlist.size();i++)
        {
            QPen pen(QColor(107,194,53));
            pen.setWidth(10);
            painter.setPen(pen);
            a=drawlist[i].first;
            b=drawlist[i].second;
            painter.drawLine(QPoint(*(pb[a]->x)+25,*(pb[a]->y)+25),QPoint(*(pb[b]->x)+25,*(pb[b]->y)+25));
        }
        return true;
    }
    if(obj == dock_widget && e->type() == QWidget::close())
    {
        dock_widget->hide();
        e->ignore();
        return true;
    }
    return QMainWindow::eventFilter(obj,e);
}
void AVL::myupdate(int x)
{
    ord.push(ord.node_poi,x);
    ord.push(ord.code_show,2);
    ord.push(ord.break_point);
    ord.push(ord.node_inpoi,x);
    ld[x]=son[x][0]!=-1 ? (ld[son[x][0]]>rd[son[x][0]] ? ld[son[x][0]]+1 : rd[son[x][0]]+1) : 0;
    rd[x]=son[x][1]!=-1 ? (ld[son[x][1]]>rd[son[x][1]] ? ld[son[x][1]]+1 : rd[son[x][1]]+1) : 0;
    size[x]=1;
    size[x]+=son[x][0]!=-1 ? size[son[x][0]] : 0;
    size[x]+=son[x][1]!=-1 ? size[son[x][1]] : 0;
    bla[x]=ld[x]-rd[x];
    return;
}
void AVL::up(int x)
{
    if(son[x][0]!=-1)
    {
        key[son[x][0]]=key[x]*2;
        ord.push(ord.node_move,son[x][0],key[son[x][0]]);
        up(son[x][0]);
    }
    if(son[x][1]!=-1)
    {
        key[son[x][1]]=key[x]*2+1;
        ord.push(ord.node_move,son[x][1],key[son[x][1]]);
        up(son[x][1]);
    }
}
void AVL::rotate(int x,int y)
{
    int k=(son[x][1]==y);
    if(son[y][k^1]!=-1)
        ord.push(ord.line_cge,son[y][k^1],y,son[y][k^1],x);
    ord.push(ord.line_cge,f[x],x,f[x],y);
    ord.push(ord.break_point);
    son[x][k]=son[y][k^1];
    f[y]=f[x];
    son[f[x]][son[f[x]][1]==x]=y;
    son[y][k^1]=x;
    f[x]=y;
    key[y]=key[x];
    ord.push(ord.node_move,y,key[y]);
    up(y);
    ord.push(ord.node_cge,f[y],-2,-1,-1,-2,son[f[y]][0],son[f[y]][1]);
    ord.push(ord.node_cge,x,-2,-1,-1,f[x],son[x][0],son[x][1]);
    ord.push(ord.node_cge,y,-2,-1,-1,f[y],son[y][0],son[y][1]);
    ord.push(ord.code_show,3);
    ord.push(ord.break_point);
    myupdate(x);
    myupdate(y);
    return;
}
void AVL::add(int x,int p)
{
    ord.push(ord.node_poi,p);
    ord.push(ord.break_point);
    ord.push(ord.node_inpoi,p);
    if(x<tree[p])
    {
        if(son[p][0]!=-1)
        {
            add(x,son[p][0]);
            myupdate(p);
            if(bla[p]>1 && p)
            {
                if(bla[son[p][0]]>=0)
                {
                    rotate(p,son[p][0]);
                }
                else
                {
                    rotate(son[p][0],son[son[p][0]][1]);
                    rotate(p,son[p][0]);
                }
            }
        }
        else
        {
            top++;
            key[top]=key[p]*2;
            tree[top]=x;
            son[p][0]=top;
            f[top]=p;
            size[top]=1;
            son[top][0]=son[top][1]=-1;
            ord.push(ord.node_add,top);
            st[top]=key[top];
            ord.push(ord.line_add,p,top);
            ord.push(ord.node_cge,p,tree[p],-1,-1,-2,son[p][0],-2);
            ord.push(ord.node_cge,top,tree[top],-1,-1,f[top],son[top][0],son[top][1]);
            ord.push(ord.node_poi,top);
            ord.push(ord.break_point);
            ord.push(ord.node_inpoi,top);
            myupdate(p);
        }
    }
    else if(x>tree[p])
    {
        if(son[p][1]!=-1)
        {
            add(x,son[p][1]);
            myupdate(p);
            if(bla[p]<-1 && p)
            {
                if(bla[son[p][1]]<=0)
                {
                    rotate(p,son[p][1]);
                }
                else
                {
                    rotate(son[p][1],son[son[p][1]][0]);
                    rotate(p,son[p][1]);
                }
            }
        }
        else
        {
            top++;
            key[top]=key[p]*2+1;
            tree[top]=x;
            son[p][1]=top;
            f[top]=p;
            size[top]=1;
            son[top][0]=son[top][1]=-1;
            ord.push(ord.node_add,top);
            st[top]=key[top];
            ord.push(ord.line_add,p,top);
            ord.push(ord.node_cge,p,tree[p],-1,-1,-2,-2,son[p][1]);
            ord.push(ord.node_cge,top,tree[top],-1,-1,f[top],son[top][0],son[top][1]);
            ord.push(ord.node_poi,top);
            ord.push(ord.break_point);
            ord.push(ord.node_inpoi,top);
            myupdate(p);
        }
    }
    return;
}
void AVL::deal()
{
    freopen("./data/intxt.txt","r",stdin);
    scanf("%d",&n);
    memset(bla,0,sizeof(bla));
    memset(f,0,sizeof(f));
    memset(son,0,sizeof(son));
    memset(ld,0,sizeof(ld));
    memset(rd,0,sizeof(rd));
    memset(size,0,sizeof(size));
    memset(tree,0,sizeof(tree));
    memset(key,0,sizeof(key));
    son[0][0]=son[0][1]=-1;
    st[0]=0;
    top=0;
    ord.push(ord.break_point);
    ord.push(ord.node_add,0);
    ord.push(ord.node_cge,0,0,-1,-1,-1,-1,-1);
    for(int i=1;i<=n;i++)
    {
        scanf("%d",&val[i]);
        ord.push(ord.code_show,1);
        ord.push(ord.break_point);
        add(val[i],0);
    }
    ord.push(ord.break_point);
    fclose(stdin);
}
AVL::~AVL()
{
    delete ui;
}
/*
 * 1.add node
 * 2.update (x) balance tree_size left_son_depth right_son_depth
 * 3.rotate
 */

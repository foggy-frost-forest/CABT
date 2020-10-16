#include "heap.h"
#include "ui_heap.h"

HEAP::HEAP(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HEAP)
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

    for(int i=1;i<=31;i++)
    {
        pb[i]=new NodeButton;
        pb[i]->setParent(show_widget);
        pb[i]->resize(50,50);
        pb[i]->init(i,treenodex[i],treenodey[i]);
        pb[i]->hide();
        pb[i]->setfa(this);
    }
    QVBoxLayout *hlay = new QVBoxLayout();
    lab_top=5;
    for(int i=1;i<=lab_top;i++)
    {
        lab[i]=new QLabel;
        lab[i]->setWordWrap(true);
        hlay->addWidget(lab[i]);
    }
    code_widget->setLayout(hlay);
    lab[1]->setText("1.add node");
    lab[2]->setText("2.if val[x] < val[father] --> swap(x,father)");
    lab[3]->setText("3.output head_node , head_node = tail_node");
    lab[4]->setText("4.if val[x] > val[left_son] < val[right_son] --> swap(x,left_son)");
    lab[5]->setText("5.if val[x] > val[right_son] < val[left_son] --> swap(x,right_son)");


    now=0;
    ord.point_init();
    connect(clock,&QTimer::timeout,[=]()mutable{
        if(now<ord.list.size())
        {
            if(ord.list[now]!=ord.break_point)
            {
                if(ord.list[now]==ord.node_add)
                {
                    pb[ord.l_nodeadd[ord.na_p].num]->show();
                    ord.na_p++;
                }
                else if(ord.list[now]==ord.node_cge)
                {
                    NodeCge tmp=ord.l_nodecge[ord.nc_p];
                    pb[tmp.num]->change(tmp.data_1,tmp.data_2,tmp.data_3,tmp.data_4,tmp.data_5,tmp.data_6);
                    ord.nc_p++;
                }
                else if(ord.list[now]==ord.node_excge)
                {
                    int a=ord.l_nodeexcge[ord.ne_p].num_fst;
                    int b=ord.l_nodeexcge[ord.ne_p].num_sec;
                    pb[0]=pb[a];
                    pb[a]=pb[b];
                    pb[b]=pb[0];
                    pb[a]->title(a);
                    pb[b]->title(b);
                    pb[a]->mymove(treenodex[a],treenodey[a]);
                    pb[b]->mymove(treenodex[b],treenodey[b]);
                    update();
                    ord.ne_p++;
                }
                else if(ord.list[now]==ord.node_del)
                {
                    pb[ord.l_nodedel[ord.nd_p].num]->hide();
                    std::vector <pair<int,int>> tmp;
                    for(unsigned int i=0;i<drawlist.size();i++)
                    {
                        if(drawlist[i].first==ord.l_nodedel[ord.nd_p].num)
                            continue;
                        if(drawlist[i].second==ord.l_nodedel[ord.nd_p].num)
                            continue;
                        tmp.push_back(drawlist[i]);
                    }
                    drawlist.clear();
                    for(unsigned int i=0;i<tmp.size();i++)
                        drawlist.push_back(tmp[i]);
                    update();
                    ord.nd_p++;
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
                else if(ord.list[now]==ord.line_del)
                {
                    int a=ord.l_linedel[ord.ld_p].num_fst;
                    int b=ord.l_linedel[ord.ld_p].num_sec;
                    std::vector <pair<int,int>> tmp;
                    for(unsigned int i=0;i<drawlist.size();i++)
                    {
                        if(drawlist[i].first==a && drawlist[i].second==b)
                            continue;
                        if(drawlist[i].first==b && drawlist[i].second==a)
                            continue;
                        tmp.push_back(drawlist[i]);
                    }
                    drawlist.clear();
                    for(unsigned int i=0;i<tmp.size();i++)
                        drawlist.push_back(tmp[i]);
                    update();
                    ord.ld_p++;
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
                else if(ord.list[now]==ord.output)
                {
                    output_lab->setText(QString::number(ord.l_output[ord.op_p].num));
                    output_lab->setStyleSheet("border:2px solid black;background-color:rgba(255,0,0,100);");
                    mysleep(500);
                    output_lab->setStyleSheet("border:2px solid black;background-color:rgba(0,0,0,0);");
                    ord.op_p++;
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
                    for(int i=1;i<=31;i++)
                    {
                        pb[i]->hide();
                        pb[i]->change(0);
                        pb[i]->init(i,treenodex[i],treenodey[i]);
                    }
                    drawlist.clear();
                    ord.point_init();
                    for(int i=1;i<=lab_top;i++)
                        lab[i]->setStyleSheet("background-color:rgba(0,0,0,0);");
                    output_lab->setText("");
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
                            pb[ord.l_nodeadd[ord.na_p].num]->show();
                            ord.na_p++;
                        }
                        else if(ord.list[now]==ord.node_cge)
                        {
                            NodeCge tmp=ord.l_nodecge[ord.nc_p];
                            pb[tmp.num]->change(tmp.data_1,tmp.data_2,tmp.data_3,tmp.data_4,tmp.data_5,tmp.data_6);
                            ord.nc_p++;
                        }
                        else if(ord.list[now]==ord.node_excge)
                        {
                            int a=ord.l_nodeexcge[ord.ne_p].num_fst;
                            int b=ord.l_nodeexcge[ord.ne_p].num_sec;
                            pb[0]=pb[a];
                            pb[a]=pb[b];
                            pb[b]=pb[0];
                            pb[a]->title(a);
                            pb[b]->title(b);
                            pb[a]->move(treenodex[a],treenodey[a]);
                            *(pb[a]->x)=treenodex[a];
                            *(pb[a]->y)=treenodey[a];
                            pb[b]->move(treenodex[b],treenodey[b]);
                            *(pb[b]->x)=treenodex[b];
                            *(pb[b]->y)=treenodey[b];
                            ord.ne_p++;
                        }
                        else if(ord.list[now]==ord.node_del)
                        {
                            pb[ord.l_nodedel[ord.nd_p].num]->hide();
                            std::vector <pair<int,int>> tmp;
                            for(unsigned int i=0;i<drawlist.size();i++)
                            {
                                if(drawlist[i].first==ord.l_nodedel[ord.nd_p].num)
                                    continue;
                                if(drawlist[i].second==ord.l_nodedel[ord.nd_p].num)
                                    continue;
                                tmp.push_back(drawlist[i]);
                            }
                            drawlist.clear();
                            for(unsigned int i=0;i<tmp.size();i++)
                                drawlist.push_back(tmp[i]);
                            ord.nd_p++;
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
                        else if(ord.list[now]==ord.line_del)
                        {
                            int a=ord.l_linedel[ord.ld_p].num_fst;
                            int b=ord.l_linedel[ord.ld_p].num_sec;
                            std::vector <pair<int,int>> tmp;
                            for(unsigned int i=0;i<drawlist.size();i++)
                            {
                                if(drawlist[i].first==a && drawlist[i].second==b)
                                    continue;
                                if(drawlist[i].first==b && drawlist[i].second==a)
                                    continue;
                                tmp.push_back(drawlist[i]);
                            }
                            drawlist.clear();
                            for(unsigned int i=0;i<tmp.size();i++)
                                drawlist.push_back(tmp[i]);
                            ord.ld_p++;
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
                    update();
                    mode=pause_mode;
                }
                else if(mode==start_mode)
                {
                    clock->stop();
                    for(int i=1;i<=31;i++)
                    {
                        pb[i]->hide();
                        pb[i]->change(0);
                        pb[i]->init(i,treenodex[i],treenodey[i]);
                    }
                    drawlist.clear();
                    ord.point_init();
                    for(int i=1;i<=lab_top;i++)
                        lab[i]->setStyleSheet("background-color:rgba(0,0,0,0);");
                    output_lab->setText("");
                    update();
                    now=0;
                    mode=pause_mode;
                }
            }
        }
    });
}
void HEAP::turn_next_mode()
{
    mode=next_mode;
    status_lab->setText("step");
    clock->start(10);
}
void HEAP::turn_run_mode()
{
    mode=run_mode;
    status_lab->setText("running");
    clock->start(10);
}
void HEAP::turn_pause_mode()
{
    mode=pause_mode;
    status_lab->setText("pause");
}
void HEAP::turn_start_mode()
{
    mode=start_mode;
    if(now==ord.list.size()) now--;
    status_lab->setText("pause");
    clock->start(10);
}
void HEAP::turn_back_mode()
{
    mode=back_mode;
    if(now==ord.list.size()) now--;
    status_lab->setText("pause");
    clock->start(10);
}
void HEAP::mysleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
bool HEAP::eventFilter(QObject *obj, QEvent *e)
{
    if(obj == show_widget && e->type() == QEvent::Paint)
    {
        QPainter painter(show_widget);
        for(unsigned int i=0,a,b;i<drawlist.size();i++)
        {
            QPen pen(QColor(107,194,53));
            pen.setWidth(10);
            painter.setPen(pen);
            a=drawlist[i].first;
            b=drawlist[i].second;
            if(a!=0 && b!=0)
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
    return QMainWindow::eventFilter(obj,e);//其它绘图事件交给父类处理
}
void HEAP::up(int x)
{
    ord.push(ord.node_poi,x);
    ord.push(ord.break_point);
    ord.push(ord.node_inpoi,x);
    if(x==1) return;
    if(tree[x]<tree[x/2])
    {
        int t=tree[x];
        tree[x]=tree[x/2];
        tree[x/2]=t;
        ord.push(ord.code_show,2);
        ord.push(ord.node_excge,x,x/2);
        ord.push(ord.break_point);
    }
    up(x/2);
}
void HEAP::down(int x)
{
    ord.push(ord.node_poi,x);
    ord.push(ord.break_point);
    ord.push(ord.node_inpoi,x);
    if(x*2>top) return;
    if(x*2==top)
    {
        ord.push(ord.code_show,4);
        ord.push(ord.break_point);
        if(tree[x*2]<tree[x])
        {
            int t=tree[x];
            tree[x]=tree[x*2];
            tree[x*2]=t;
            ord.push(ord.node_excge,x,x*2);
            ord.push(ord.break_point);
        }
        return;
    }
    if(tree[x*2]>tree[x*2+1])
    {
        ord.push(ord.code_show,5);
        ord.push(ord.break_point);
        if(tree[x]>tree[x*2+1])
        {
            ord.push(ord.node_excge,x,x*2+1);
            ord.push(ord.break_point);
            int t=tree[x];
            tree[x]=tree[x*2+1];
            tree[x*2+1]=t;
            down(x*2+1);
        }
        return;
    }
    else
    {
        ord.push(ord.code_show,4);
        ord.push(ord.break_point);
        if(tree[x]>tree[x*2])
        {
            ord.push(ord.node_excge,x,x*2);
            ord.push(ord.break_point);
            int t=tree[x];
            tree[x]=tree[x*2];
            tree[x*2]=t;
            down(x*2);
        }
        return;
    }
}
void HEAP::deal()
{
    freopen("./data/intxt.txt","r",stdin);
    scanf("%d",&n);
    top=0;
    memset(tree,0,sizeof(tree));
    ord.push(ord.break_point);
    for(int i=1;i<=n;i++)
    {
        scanf("%d",&m);
        if(m==1)
        {
            scanf("%d",&k);
            top++;
            tree[top]=k;
            ord.push(ord.node_add,top);
            ord.push(ord.node_cge,top,k,-1,-1,-1,-1,-1);
            ord.push(ord.code_show,1);
            if(top!=1) ord.push(ord.line_add,top,top/2);
            ord.push(ord.break_point);
            up(top);
        }
        else
        {
            ord.push(ord.code_show,3);
            ord.push(ord.line_del,top,top/2);
            ord.push(ord.line_del,1,2);
            ord.push(ord.line_del,1,3);
            ord.push(ord.break_point);
            ord.push(ord.output,tree[1]);
            ord.push(ord.node_excge,top,1);
            ord.push(ord.node_del,top);
            if(top>2)
                ord.push(ord.line_add,1,2);
            if(top>3)
                ord.push(ord.line_add,1,3);
            ord.push(ord.break_point);
            tree[1]=tree[top];
            top--;
            down(1);
        }
    }
    ord.push(ord.break_point);
    fclose(stdin);
}
HEAP::~HEAP()
{
    delete ui;
}

/*
 * 伪代码
 * 1.add node
 * 2.if val[x] < val[father] --> swap(x,father)
 * 3.output head_node , head_node = tail_node
 * 4.if val[x] > val[left_son] < val[right_son] --> swap(x,left_son)
 * 5.if val[x] > val[right_son] < val[left_son] --> swap(x,right_son)
 */

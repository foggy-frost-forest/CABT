#include "pst.h"
#include "ui_pst.h"
PST::PST(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PST)
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

    for(int i=1;i<=25;i++)
    {
        pb[i]=new NodeButton;
        pb[i]->setParent(show_widget);
        pb[i]->resize(50,50);
        pb[i]->init(i,treenodex[i],treenodey[i]);
        pb[i]->hide();
        pb[i]->setfa(this);
    }
    QVBoxLayout *hlay = new QVBoxLayout();
    lab_top=8;
    for(int i=1;i<=lab_top;i++)
    {
        lab[i]=new QLabel;
        lab[i]->setWordWrap(true);
        hlay->addWidget(lab[i]);
    }
    code_widget->setLayout(hlay);
    lab[1]->setText("1.creat segment tree");
    lab[2]->setText("2.insert");
    lab[3]->setText("3.left == right --> retrun");
    lab[4]->setText("4.left_son = new node , right_son = old node");
    lab[5]->setText("5.right_son = new node , left_son = old node");
    lab[6]->setText("6.find x-th node in node[1~8]");
    lab[7]->setText("7.sum(val[1~mid]) >= x --> find x in left_son");
    lab[8]->setText("8.sum(val[1~mid]) < x --> find x-sum(val[1~mid]) in right_son");

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
                    for(int i=1;i<=25;i++)
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
                    for(int i=1;i<=25;i++)
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
void PST::turn_next_mode()
{
    mode=next_mode;
    status_lab->setText("step");
    clock->start(10);
}
void PST::turn_run_mode()
{
    mode=run_mode;
    status_lab->setText("running");
    clock->start(10);
}
void PST::turn_pause_mode()
{
    mode=pause_mode;
    status_lab->setText("pause");
}
void PST::turn_start_mode()
{
    mode=start_mode;
    if(now==ord.list.size()) now--;
    status_lab->setText("pause");
    clock->start(10);
}
void PST::turn_back_mode()
{
    mode=back_mode;
    if(now==ord.list.size()) now--;
    status_lab->setText("pause");
    clock->start(10);
}
void PST::mysleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
bool PST::eventFilter(QObject *obj, QEvent *e)
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
void PST::creat(int l,int r,int x)
{
    ord.push(ord.node_poi,x);
    ord.push(ord.node_add,x);
    ord.push(ord.node_cge,x,0,l,r,x==1 ? 0 : x/2,x<=3 ? x*2 : -1,x<=3 ? x*2+1 : -1);
    if(x!=1) ord.push(ord.line_add,x,x/2);
    ord.push(ord.break_point);
    tree[x].w=0;
    tree[x].lef=l;
    tree[x].rig=r;
    if(l==r)
    {
        tree[x].lson=0;
        tree[x].rson=0;
        ord.push(ord.node_inpoi,x);
        ord.push(ord.break_point);
        return;
    }
    int mid=(l+r)/2;
    tree[x].lson=x*2;
    tree[x].rson=x*2+1;
    creat(l,mid,x*2);
    creat(mid+1,r,x*2+1);
    ord.push(ord.node_inpoi,x);
    ord.push(ord.break_point);
    return;
}
void PST::insert(int l,int r,int x,int p)
{
    top++;
    int tmp=top;
    tree[top]=tree[p];
    tree[top].w=tree[p].w+1;
    ord.push(ord.node_poi,tmp);
    ord.push(ord.node_poi,p);
    ord.push(ord.node_add,tmp);
    ord.push(ord.node_cge,tmp,0,l,r,r-l==3 ? 0 : tmp-1);
    ord.push(ord.break_point);
    if(l==r)
    {
        ord.push(ord.code_show,3);
        tree[top].lson=tree[top].rson=-1;
    }
    else
    {
        int mid=(l+r)/2;
        if(x<=mid)
        {
            ord.push(ord.code_show,4);
            tree[top].lson=top+1;
            ord.push(ord.line_add,tmp,tree[tmp].lson);
            ord.push(ord.line_add,tmp,tree[tmp].rson);
            insert(l,mid,x,tree[p].lson);
        }
        else
        {
            ord.push(ord.code_show,5);
            tree[top].rson=top+1;
            ord.push(ord.line_add,tmp,tree[tmp].lson);
            ord.push(ord.line_add,tmp,tree[tmp].rson);
            insert(mid+1,r,x,tree[p].rson);
        }
    }
    ord.push(ord.node_cge,tmp,tree[tmp].w,-2,-2,-2,tree[tmp].lson,tree[tmp].rson);
    ord.push(ord.node_inpoi,tmp);
    ord.push(ord.node_inpoi,p);
    ord.push(ord.break_point);
    return;
}
int PST::ask(int x,int y,int z)
{
    ord.push(ord.node_poi,y);
    ord.push(ord.node_poi,z);
    ord.push(ord.break_point);
    if(tree[y].lef==tree[y].rig)
    {
        ord.push(ord.node_inpoi,y);
        ord.push(ord.node_inpoi,z);
        ord.push(ord.break_point);
        return tree[y].lef;
    }
    int tmp,k=tree[tree[z].lson].w-tree[tree[y].lson].w;
    if(k>=x)
    {
        ord.push(ord.code_show,7);
        tmp=ask(x,tree[y].lson,tree[z].lson);
        ord.push(ord.node_inpoi,y);
        ord.push(ord.node_inpoi,z);
        ord.push(ord.break_point);
    }
    else
    {
        ord.push(ord.code_show,8);
        tmp=ask(x-k,tree[y].rson,tree[z].rson);
        ord.push(ord.node_inpoi,y);
        ord.push(ord.node_inpoi,z);
        ord.push(ord.break_point);
    }
    return tmp;
}
void PST::deal()
{
    freopen("./data/intxt.txt","r",stdin);
    scanf("%d%d",&n,&m);
    for(int i=1;i<=n;i++)
        scanf("%d",&a[i]);
    ord.push(ord.break_point);
    ord.push(ord.code_show,1);
    top=7;
    creat(1,4,1);
    head[0]=1;
    ord.push(ord.code_show,2);
    ord.push(ord.break_point);
    for(int i=1;i<=n;i++)
    {
        head[i]=top+1;
        insert(1,4,a[i],head[i-1]);
    }
    int l,r,c;
    ord.push(ord.code_show,6);
    ord.push(ord.break_point);
    for(int i=1;i<=m;i++)
    {
        scanf("%d%d%d",&l,&r,&c);
        ans=ask(c,head[l-1],head[r]);
        ord.push(ord.output,ans);
    }
    ord.push(ord.break_point);
    fclose(stdin);
}
PST::~PST()
{
    delete ui;
}

/*
伪代码
1.creat segment tree
2.insert
3.left == right --> retrun
4.left_son = new node , right_son = old node
5.right_son = new node , left_son = old node
6.find x-th node in node[1~8]
7.sum(val[1~mid]) >= x --> find x in left_son
8.sum(val[1~mid]) < x --> find x-sum(val[1~mid]) in right_son
*/

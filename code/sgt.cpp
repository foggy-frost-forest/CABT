#include "sgt.h"
#include "ui_sgt.h"
#include "infowidget.h"

SGT::SGT(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SGT)
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
    lab_top=6;
    for(int i=1;i<=lab_top;i++)
    {
        lab[i]=new QLabel;
        lab[i]->setWordWrap(true);
        hlay->addWidget(lab[i]);
    }
    code_widget->setLayout(hlay);
    lab[1]->setText("1.creat tree");
    lab[2]->setText("2.add x");
    lab[3]->setText("3.update num[x] lazy[x] pushdown(x)");
    lab[4]->setText("4.add(l,mid) add(mid+1,r)");
    lab[5]->setText("5.find x");
    lab[6]->setText("6,find(l,mid) find(mid+1,r)");


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
                    pb[tmp.num]->info->get_lab(4)->setText("LAZY:"+QString::number(tmp.data_4));
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
                            pb[tmp.num]->info->get_lab(4)->setText("LAZY:"+QString::number(tmp.data_4));
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
void SGT::turn_next_mode()
{
    mode=next_mode;
    status_lab->setText("step");
    clock->start(10);
}
void SGT::turn_run_mode()
{
    mode=run_mode;
    status_lab->setText("running");
    clock->start(10);
}
void SGT::turn_pause_mode()
{
    mode=pause_mode;
    status_lab->setText("pause");
}
void SGT::turn_start_mode()
{
    mode=start_mode;
    if(now==ord.list.size()) now--;
    status_lab->setText("pause");
    clock->start(10);
}
void SGT::turn_back_mode()
{
    mode=back_mode;
    if(now==ord.list.size()) now--;
    status_lab->setText("pause");
    clock->start(10);
}
void SGT::mysleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
bool SGT::eventFilter(QObject *obj, QEvent *e)
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
void SGT::add(int l,int r,int p,int s,int e,int k)
{
    ord.push(ord.node_poi,p);
    ord.push(ord.break_point);
    if(l==s && r==e)
    {
        num[p]+=(r-l+1)*k;
        lazy[p]+=k;
        ord.push(ord.node_cge,p,num[p],-2,-2,lazy[p],-2,-2);
        ord.push(ord.node_inpoi,p);
        return;
    }
    int mid=(l+r)>>1;
    num[p*2]+=lazy[p]*(mid-l+1);
    lazy[p*2]+=lazy[p];
    num[p*2+1]+=lazy[p]*(r-mid);
    lazy[p*2+1]+=lazy[p];
    lazy[p]=0;
    ord.push(ord.node_cge,p,num[p],-2,-2,lazy[p],-2,-2);
    ord.push(ord.node_cge,p*2,num[p*2],-2,-2,lazy[p*2],-2,-2);
    ord.push(ord.node_cge,p*2+1,num[p*2+1],-2,-2,lazy[p*2+1],-2,-2);
    ord.push(ord.code_show,3);
    ord.push(ord.break_point);
    ord.push(ord.code_show,4);
    ord.push(ord.break_point);
    if(e<=mid)
    {
        add(l,mid,p*2,s,e,k);
    }
    else if(mid<s)
    {
        add(mid+1,r,p*2+1,s,e,k);
    }
    else
    {
        add(l,mid,p*2,s,mid,k);
        add(mid+1,r,p*2+1,mid+1,e,k);
    }
    num[p]=num[p*2]+num[p*2+1];
    ord.push(ord.code_show,3);
    ord.push(ord.node_cge,p,num[p],-2,-2,-2,-2,-2);
    ord.push(ord.node_inpoi,p);
    ord.push(ord.break_point);
    return;
}
int SGT::find(int l,int r,int p,int s,int e)
{
    ord.push(ord.node_poi,p);
    ord.push(ord.break_point);
    if(l==s && r==e)
    {
        ord.push(ord.node_inpoi,p);
        return num[p];
    }
    int mid=(l+r)>>1,k=0;
    num[p*2]+=lazy[p]*(mid-l+1);
    lazy[p*2]+=lazy[p];
    num[p*2+1]+=lazy[p]*(r-mid);
    lazy[p*2+1]+=lazy[p];
    lazy[p]=0;
    ord.push(ord.node_cge,p,num[p],-2,-2,lazy[p],-2,-2);
    ord.push(ord.node_cge,p*2,num[p*2],-2,-2,lazy[p*2],-2,-2);
    ord.push(ord.node_cge,p*2+1,num[p*2+1],-2,-2,lazy[p*2+1],-2,-2);
    ord.push(ord.code_show,3);
    ord.push(ord.break_point);
    ord.push(ord.code_show,6);
    ord.push(ord.break_point);
    if(e<=mid)
    {
        k=find(l,mid,p*2,s,e);
    }
    else if(mid<s)
    {
        k=find(mid+1,r,p*2+1,s,e);
    }
    else
    {
        k+=find(l,mid,p*2,s,mid);
        k+=find(mid+1,r,p*2+1,mid+1,e);
    }
    ord.push(ord.node_inpoi,p);
    ord.push(ord.break_point);
    return k;
}
void SGT::creat(int l,int r,int p)
{
    ord.push(ord.node_poi,p);
    ord.push(ord.node_add,p);
    ord.push(ord.node_cge,p,0,l,r,0,l==r ? -1 : p*2,l==r ? -1 : p*2+1);
    if(p!=1)
        ord.push(ord.line_add,p,p/2);
    ord.push(ord.break_point);

    if(l==r)
    {
        ord.push(ord.node_inpoi,p);
        ord.push(ord.break_point);
        return;
    }
    int mid=(l+r)>>1;
    creat(l,mid,p*2);
    creat(mid+1,r,p*2+1);
    ord.push(ord.node_inpoi,p);
    ord.push(ord.break_point);
}
void SGT::deal()
{
    int k,s,e,ans,top=1;
    freopen("./data/intxt.txt","r",stdin);
    scanf("%d",&n);
    memset(num,0,sizeof(num));
    memset(lazy,0,sizeof(lazy));
    ord.push(ord.break_point);
    while(top<n) top*=2;
    ord.push(ord.code_show,1);
    ord.push(ord.break_point);
    creat(1,top,1);
    for(int i=1;i<=n;i++)
    {
        scanf("%d",&m);
        if(m==1)
        {
            scanf("%d%d%d",&s,&e,&k);
            ord.push(ord.code_show,2);
            ord.push(ord.break_point);
            add(1,top,1,s,e,k);
        }
        else
        {
            ord.push(ord.code_show,5);
            ord.push(ord.break_point);
            scanf("%d%d",&s,&e);
            ans=find(1,top,1,s,e);
            ord.push(ord.output,ans);
        }
    }
    ord.push(ord.break_point);
    fclose(stdin);
}
SGT::~SGT()
{
    delete ui;
}

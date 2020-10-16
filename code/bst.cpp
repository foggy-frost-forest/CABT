#include "bst.h"
#include "ui_bst.h"

BST::BST(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BST)
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
    }
    QVBoxLayout *hlay = new QVBoxLayout();
    lab_top=9;
    for(int i=1;i<=lab_top;i++)
    {
        lab[i]=new QLabel;
        lab[i]->setWordWrap(true);
        hlay->addWidget(lab[i]);
    }
    code_widget->setLayout(hlay);
    lab[1]->setText("1.add_node(x,point=head");
    lab[2]->setText("2.x<fa --> add_node(x,point=fa->left_son)");
    lab[3]->setText("3.x>fa --> add_node(x,point=fa->Right_son)");
    lab[4]->setText("4.node[point] = new node");
    lab[5]->setText("5.find_node(x,point=head)");
    lab[6]->setText("6.x<fa --> find(x,point=fa->left_son)");
    lab[7]->setText("7.x>fa --> find(x,point=fa->Right_son)");
    lab[8]->setText("8.x==fa --> print(true)");
    lab[9]->setText("9.x==NULL --> print(false)");


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
                    if(ord.l_output[ord.op_p].num)
                        output_lab->setText("True");
                    else
                        output_lab->setText("False");
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
void BST::turn_next_mode()
{
    mode=next_mode;
    status_lab->setText("step");
    clock->start(10);
}
void BST::turn_run_mode()
{
    mode=run_mode;
    status_lab->setText("running");
    clock->start(10);
}
void BST::turn_pause_mode()
{
    mode=pause_mode;
    status_lab->setText("pause");
}
void BST::turn_start_mode()
{
    mode=start_mode;
    if(now==ord.list.size()) now--;
    status_lab->setText("pause");
    clock->start(10);
}
void BST::turn_back_mode()
{
    mode=back_mode;
    if(now==ord.list.size()) now--;
    status_lab->setText("pause");
    clock->start(10);
}
void BST::mysleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
bool BST::eventFilter(QObject *obj, QEvent *e)
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

void BST::add(int x,int p)
{
    ord.push(ord.node_poi,p);
    //if(p>=32) return;
    if(!tree[p])
    {
        tree[p]=x;
        ord.push(ord.code_show,4);
        ord.push(ord.node_cge,p,tree[p],-1,-1,-2);
        ord.push(ord.node_add,p);
        if(p/2>0)
        {
            ord.push(ord.line_add,p/2,p);
            if(p/2*2==p)
                ord.push(ord.node_cge,p/2,tree[p/2],-1,-1,-2,p,-2);
            else
                ord.push(ord.node_cge,p/2,tree[p/2],-1,-1,-2,-2,p);
        }
        ord.push(ord.break_point);
        ord.push(ord.node_inpoi,p);
        return;
    }
    ord.push(ord.break_point);
    ord.push(ord.node_inpoi,p);
    if(x<tree[p])
    {
        add(x,p*2);
        ord.push(ord.code_show,2);
    }
    else if(x>tree[p])
    {
        add(x,p*2+1);
        ord.push(ord.code_show,3);
    }
    return;
}
bool BST::find(int x,int p)
{
    ord.push(ord.node_poi,p);
    ord.push(ord.break_point);
    ord.push(ord.node_inpoi,p);
    if(!tree[p])
    {
        ord.push(ord.code_show,9);
        ord.push(ord.break_point);
        return false;
    }
    if(tree[p]==x)
    {
        ord.push(ord.code_show,8);
        ord.push(ord.break_point);
        return true;
    }
    if(x<tree[p])
    {
        ord.push(ord.code_show,6);
        return find(x,p*2);
    }
    else if(x>tree[p])
    {
        ord.push(ord.code_show,7);
        return find(x,p*2+1);
    }
    return false;
}
void BST::deal()
{
    int a,b;
    freopen("./data/intxt.txt","r",stdin);
    scanf("%d%d",&n,&m);
    ord.push(ord.break_point);
    memset(tree,0,sizeof(tree));
    ord.push(ord.code_show,1);
    ord.push(ord.break_point);
    for(int i=1;i<=n;i++)
    {
        scanf("%d",&a);
        add(a,1);
    }
    ord.push(ord.code_show,5);
    ord.push(ord.break_point);
    for(int i=1;i<=m;i++)
    {
        scanf("%d",&b);
        if(find(b,1))
        {
            ord.push(ord.output,1);
        }
        else
        {
            ord.push(ord.output,0);
        }
    }
    ord.push(ord.break_point);
    fclose(stdin);
}
BST::~BST()
{
    delete ui;
}

/*伪代码
 * 1.add_node(x,point=head)
 * 2.x<fa --> add_node(x,point=fa->left_son)
 * 3.x>fa --> add_node(x,point=fa->Right_son)
 * 4.node[point] = new node
 * 5.find_node(x,point=head)
 * 6.x<fa --> find(x,point=fa->left_son)
 * 7.x>fa --> find(x,point=fa->Right_son)
 * 8.x==fa --> print(true)
 * 9.x==NULL --> print(false)
 */

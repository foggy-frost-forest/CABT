#ifndef ORDERLIST_H
#define ORDERLIST_H

#include <vector>

struct NodeAdd
{
    int num;
    NodeAdd(int num) : num(num){}
};
struct NodeDel
{
    int num;
    NodeDel(int num) : num(num){}
};
struct NodeCge
{
    int num,data_1,data_2,data_3,data_4,data_5,data_6;
    NodeCge(int num,int d1,int d2,int d3,int d4,int d5,int d6) : num(num),data_1(d1),data_2(d2),data_3(d3),data_4(d4),data_5(d5),data_6(d6){}
};
struct NodeMove
{
    int num_from,num_to;
    NodeMove(int num_from,int num_to) : num_from(num_from),num_to(num_to){}
};
struct NodeExcge
{
    int num_fst,num_sec;
    NodeExcge(int num_fst,int num_sec) : num_fst(num_fst),num_sec(num_sec){}
};
struct LineAdd
{
    int num_fst,num_sec;
    LineAdd(int num_fst,int num_sec) : num_fst(num_fst),num_sec(num_sec){}
};
struct LineDel
{
    int num_fst,num_sec;
    LineDel(int num_fst,int num_sec) : num_fst(num_fst),num_sec(num_sec){}
};
struct NodePoi
{
    int num;
    NodePoi(int num) : num(num){}
};
struct NodeInpoi
{
    int num;
    NodeInpoi(int num) : num(num){}
};
struct CodeShow
{
    int num;
    CodeShow(int num) : num(num){}
};
struct OutPut
{
    int num;
    OutPut(int num) : num(num){}
};
struct LineCge
{
    int old_fst,old_sec,new_fst,new_sec;
    LineCge(int  ofst,int osec,int nfst,int nsec) : old_fst(ofst),old_sec(osec),new_fst(nfst),new_sec(nsec){}
};
class OrderList
{
public:
    enum order_type {break_point,node_add,node_del,node_cge,node_move,node_excge,line_add,line_del,node_poi,node_inpoi,output,code_show,line_cge};
    std::vector <order_type> list;
    std::vector <NodeAdd> l_nodeadd;
    std::vector <NodeDel> l_nodedel;
    std::vector <NodeCge> l_nodecge;
    std::vector <NodeMove> l_nodemove;
    std::vector <NodeExcge> l_nodeexcge;
    std::vector <LineAdd> l_lineadd;
    std::vector <LineDel> l_linedel;
    std::vector <NodePoi> l_nodepoi;
    std::vector <NodeInpoi> l_nodeinpoi;
    std::vector <OutPut> l_output;
    std::vector <CodeShow> l_codeshow;
    std::vector <LineCge> l_linecge;
    int na_p,nd_p,nc_p,nm_p,ne_p,la_p,ld_p,np_p,nip_p,op_p,cs_p,lc_p;
    void point_init()
    {
        na_p=nd_p=nc_p=nm_p=ne_p=la_p=ld_p=np_p=nip_p=op_p=cs_p=lc_p=0;
    }
    void push(order_type x,int d1=-1,int d2=-1,int d3=-1,int d4=-1,int d5=-1,int d6=-1,int d7=-1)
    {
        list.push_back(x);
        switch(x)
        {
        case break_point:
            break;
        case node_add:
            l_nodeadd.push_back(NodeAdd(d1));
            break;
        case node_del:
            l_nodedel.push_back(NodeDel(d1));
            break;
        case node_cge:
            l_nodecge.push_back(NodeCge(d1,d2,d3,d4,d5,d6,d7));
            break;
        case node_move:
            l_nodemove.push_back(NodeMove(d1,d2));
            break;
        case node_excge:
            l_nodeexcge.push_back(NodeExcge(d1,d2));
            break;
        case line_add:
            l_lineadd.push_back(LineAdd(d1,d2));
            break;
        case line_del:
            l_linedel.push_back(LineDel(d1,d2));
            break;
        case node_poi:
            l_nodepoi.push_back(NodePoi(d1));
            break;
        case node_inpoi:
            l_nodeinpoi.push_back(NodeInpoi(d1));
            break;
        case output:
            l_output.push_back(OutPut(d1));
            break;
        case code_show:
            l_codeshow.push_back(CodeShow(d1));
            break;
        case line_cge:
            l_linecge.push_back(LineCge(d1,d2,d3,d4));
            break;
        default:
            break;
        }
    }
};
#endif // ORDERLIST_H

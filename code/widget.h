#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QString>
#include "avl.h"
#include "bst.h"
#include "heap.h"
#include "sgt.h"
#include "pst.h"
#include "ackmp.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    int flag;
    QString example,describe;
    BST *bst_son;
    AVL *avl_son;
    SGT *sgt_son;
    HEAP *heap_son;
    PST *pst_son;
    ACKMP *ackmp_son;
    void judge();
    Ui::Widget *ui;
};
#endif // WIDGET_H

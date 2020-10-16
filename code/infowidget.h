#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QCloseEvent>
#include <QLabel>

namespace Ui {
class InfoWidget;
}

class InfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit InfoWidget(QWidget *parent = nullptr);
    ~InfoWidget();
    int title;
    void closeEvent(QCloseEvent *event);
    void change(int d1=-1,int d2=-1,int d3=-1,int d4=-1,int d5=-1,int d6=-1);
    QLabel *get_lab(int x);
private:
    Ui::InfoWidget *ui;
};

#endif // INFOWIDGET_H

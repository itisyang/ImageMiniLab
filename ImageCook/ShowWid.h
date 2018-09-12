#ifndef SHOWWID_H
#define SHOWWID_H

#include <QWidget>
#include <QAction>
#include <QMenu>
#include <QContextMenuEvent>

#include "define.h"

class ShowWid : public QWidget
{
    Q_OBJECT

public:
    ShowWid(QWidget *parent = Q_NULLPTR);
    ~ShowWid();

private slots:
    void on_actSharpen_triggered();             //锐化
    void on_actDenoise_triggered();             //去噪
    void on_actGrayscale_triggered();           //灰度化
    void On_menuRotate_triggered(QAction *act); //旋转

    //void on_actRotate(QAction *act);

    void On_SetBasePix(QPixmap pix);            //原图
    void On_Undo();                             //撤销
private:
    void paintEvent(QPaintEvent *event);                //绘制事件
    void contextMenuEvent(QContextMenuEvent *event);    //右键菜单

private:
    QPixmap m_pixBase;          //原始图像
    QPixmap m_pixShow;          //效果显示图像

    int m_nRotateDegrees;       //旋转角度
    bool m_bGrayscale;          //灰度标志

    QMenu m_menuMouse;          //鼠标右键菜单
    QAction m_actSharpen;       //锐化
    QAction m_actDenoise;       //去噪
    QAction m_actGrayscale;     //灰度化
    QAction m_actRotateMenu;    //旋转
    QMenu m_menuRotate;         //旋转
    QAction m_actRotateRight;   //向右旋转
    QAction m_actRotateLeft;    //向左旋转

    QList<int> m_listStepHistory;      //历史操作步骤

};

#endif // SHOWWID_H

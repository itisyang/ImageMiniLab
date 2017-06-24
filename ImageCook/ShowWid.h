#ifndef SHOWWID_H
#define SHOWWID_H

#include <QWidget>
#include <QAction>
#include <QMenu>
#include <QContextMenuEvent>

class ShowWid : public QWidget
{
    Q_OBJECT

public:
    ShowWid(QWidget *parent = Q_NULLPTR);
    ~ShowWid();

public slots:
    void On_SetBasePix(QPixmap pix);

private slots:
    void on_actSharpen_triggered();
    void on_actDenoise_triggered();
    void on_actGrayscale_triggered();
    void On_menuRotate_triggered(QAction *act);

    //void on_actRotate(QAction *act);
private:
    void paintEvent(QPaintEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);

private:
    QPixmap m_pixBase;//原始图片
    QPixmap m_pixShow;//显示效果图片

    int m_nRotateDegrees;
    bool m_bGrayscale;

    QMenu m_menuMouse;//鼠标右键菜单
    QAction m_actSharpen;//锐化
    QAction m_actDenoise;//去噪
    QAction m_actGrayscale;//灰度化
    QAction m_actRotateMenu;
    QMenu m_menuRotate;//旋转
    QAction m_actRotateRight;//向右旋转
    QAction m_actRotateLeft;//向左旋转



};

#endif // SHOWWID_H

#include <QPainter>
#include <QMatrix>
#include <QDebug>
#include <QMetaObject>
#include <QImage>

#include "ShowWid.h"

ShowWid::ShowWid(QWidget *parent)
{
    m_nRotateDegrees = 0;//初始旋转角度
    m_bGrayscale = false;

    m_actSharpen.setText("锐化");
    m_actDenoise.setText("去噪");
    m_actGrayscale.setText("灰度化");
    m_actRotateMenu.setText("旋转");
    m_actRotateMenu.setMenu(&m_menuRotate);
    m_actRotateLeft.setText("向左旋转");
    m_actRotateRight.setText("向右旋转");

    QList<QAction *> listMenuRotate;
    listMenuRotate << &m_actRotateLeft << &m_actRotateRight;
    m_menuRotate.addActions(listMenuRotate);

    QList<QAction *> listMenuMouse;
    listMenuMouse << &m_actSharpen << &m_actDenoise << &m_actGrayscale << &m_actRotateMenu;
    m_menuMouse.addActions(listMenuMouse);

    connect(&m_actSharpen, SIGNAL(triggered()), this, SLOT(on_actSharpen_triggered()));
    connect(&m_actDenoise, SIGNAL(triggered()), this, SLOT(on_actDenoise_triggered()));
    connect(&m_actGrayscale, SIGNAL(triggered()), this, SLOT(on_actGrayscale_triggered()));
    connect(&m_menuRotate, SIGNAL(triggered(QAction*)), this, SLOT(On_menuRotate_triggered(QAction*)));

}

ShowWid::~ShowWid()
{

}

void ShowWid::paintEvent(QPaintEvent *event)
{
    QPainter painter;

    if (m_pixBase.width() && m_pixBase.height())
    {
        QPixmap pix;
        //旋转处理
        if (m_nRotateDegrees)
        {
            QMatrix matrix;
            matrix.rotate(m_nRotateDegrees);
            pix = m_pixShow.transformed(matrix, Qt::SmoothTransformation);
        }
        else
        {
            pix = m_pixShow;
        }

        //按窗口大小缩放
        pix = pix.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        painter.begin(this);
        painter.drawPixmap(0, 0, pix.width(), pix.height(), pix);
    }
    
}

void ShowWid::contextMenuEvent(QContextMenuEvent *event)
{
    //鼠标右键菜单出现的位置为当前鼠标的位置
    if (m_bGrayscale)
    {
        m_actGrayscale.setText("彩色化");
    }
    else
    {
        m_actGrayscale.setText("灰度化");
    }
    m_menuMouse.exec(QCursor::pos());
    event->accept();
}

void ShowWid::On_SetBasePix(QPixmap pix)
{
    m_pixBase = pix;
    m_pixShow = m_pixBase;
}


void ShowWid::on_actSharpen_triggered()
{
    qDebug() << "on_actSharpen_triggered";
}

void ShowWid::on_actDenoise_triggered()
{

}

void ShowWid::on_actGrayscale_triggered()
{
    qDebug() << "on_actGrayscale_triggered";
    m_bGrayscale = !m_bGrayscale;

    if (m_bGrayscale)
    {
        QImage img = m_pixShow.toImage();
        if( img.allGray() )
        {
            qDebug() << "allGray";
        }
        else
        {
            QColor oldColor;
            QImage * newImage = new QImage(img.width(), img.height(), QImage::Format_ARGB32);

            for(int x = 0; x < newImage->width(); x++){
                for(int y = 0; y < newImage->height(); y++){
                    oldColor = QColor(img.pixel(x,y));
                    int average = (oldColor.red()+oldColor.green()+oldColor.blue())/3;
                    newImage->setPixel(x, y, qRgb(average,average,average));
                }
            }
            m_pixShow = QPixmap::fromImage(*newImage);
            delete newImage;
        }

    }
    else
    {
        m_pixShow = m_pixBase;
    }

    update();
}

void ShowWid::On_menuRotate_triggered(QAction *act)
{
    if (act == &m_actRotateRight)
    {
        m_nRotateDegrees += -90;
        if (m_nRotateDegrees == 360 || m_nRotateDegrees == -360)
        {
            m_nRotateDegrees = 0;
        }
    }
    else if (act == &m_actRotateLeft)
    {
        m_nRotateDegrees += 90;
        if (m_nRotateDegrees == 360 || m_nRotateDegrees == -360)
        {
            m_nRotateDegrees = 0;
        }
    }

    update();
}

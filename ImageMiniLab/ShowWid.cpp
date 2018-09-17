#include <QPainter>
#include <QMatrix>
#include <QDebug>
#include <QMetaObject>
#include <QImage>

#include "ShowWid.h"

#pragma execution_character_set("utf-8")

ShowWid::ShowWid(QWidget *parent): m_fScale(1), xtranslate(0), ytranslate(0)
{
    m_nRotateDegrees = 0;//初始旋转角度
    m_bGrayscale = false;
    m_listStepHistory.clear();

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


}

ShowWid::~ShowWid()
{

}


bool ShowWid::Init()
{
    QList<bool> listRet;

    listRet << connect(&m_actSharpen, &QAction::triggered, this, &ShowWid::on_actSharpen_triggered);
    listRet << connect(&m_actDenoise, &QAction::triggered, this, &ShowWid::on_actDenoise_triggered);
    listRet << connect(&m_actGrayscale, &QAction::triggered, this, &ShowWid::on_actGrayscale_triggered);
    listRet << connect(&m_menuRotate, &QMenu::triggered, this, &ShowWid::On_menuRotate_triggered);

    for (bool ret : listRet)
    {
        if (!ret)
        {
            qDebug() << "显示界面信号槽初始化失败";
            return false;
        }
    }

    return true;
}

//绘制事件
void ShowWid::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setBrush(QBrush(QColor(121, 121, 121)));
    painter.drawRect(0, 0, this->width(), this->height());

    if (m_stImage.width() && m_stImage.height())
    {
        painter.drawImage(QPoint(xtranslate, ytranslate), m_stImage.scaled(this->width() * m_fScale, this->height() * m_fScale, Qt::KeepAspectRatio));
    }
}


//右键菜单
void ShowWid::contextMenuEvent(QContextMenuEvent *event)
{
    //鼠标右键菜单出现的位置为当前鼠标的位置
    if (m_bGrayscale)
    {
        m_actGrayscale.setText("彩色化");
        m_listStepHistory.push_back(STEP_UNGRAYSCALE);
    }
    else
    {
        m_actGrayscale.setText("灰度化");
        m_listStepHistory.push_back(STEP_GRAYSCALE);
    }
    m_menuMouse.exec(QCursor::pos());
    event->accept();
}

void ShowWid::wheelEvent(QWheelEvent *e)
{
    int numDegrees = e->delta();
    if (numDegrees > 0)
    {
        zoomout();
    }
    if (numDegrees < 0)
    {
        zoomin();
    }
    update();
}

void ShowWid::mouseMoveEvent(QMouseEvent * e)
{

}

void ShowWid::mousePressEvent(QMouseEvent * e)
{

}

void ShowWid::mouseDoubleClickEvent(QMouseEvent *e)
{

}

void ShowWid::zoomout()
{
    if (m_fScale <= 100)
    {
        m_fScale *= 1.2;
    }
    update();
}

void ShowWid::zoomin()
{
    if (m_fScale >= 0.01)
    {
        m_fScale *= 1 / 1.2;
    }
    update();
}

// void ShowWid::OpenImage(QImage& stImage)
// {
//     m_stImage = stImage;
// }

void ShowWid::OpenImage(QString strImagePath)
{
    bool ret = m_stImage.load(strImagePath);
    if (!ret)
    {
        qDebug() << "打开图片失败";
    }
    update();
}


//锐化
void ShowWid::on_actSharpen_triggered()
{
    qDebug() << "on_actSharpen_triggered";
    m_listStepHistory.push_back(STEP_SHARPE);
}

//去噪
void ShowWid::on_actDenoise_triggered()
{
    m_listStepHistory.push_back(STEP_DENOISE);
}

//灰度化
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
                    //对坐标像素点 红绿蓝 取平均值，三值相等即为灰度图
                    oldColor = QColor(img.pixel(x,y));
                    int average = (oldColor.red()+oldColor.green()+oldColor.blue())/3;
                    newImage->setPixel(x, y, qRgb(average,average,average));
                }
            }
            m_pixShow = QPixmap::fromImage(*newImage);
            delete newImage;
        }

        m_listStepHistory.push_back(STEP_GRAYSCALE);

    }
    else
    {
        m_pixShow = m_pixBase;
        m_listStepHistory.push_back(STEP_UNGRAYSCALE);
    }

    update();
}

//旋转
void ShowWid::On_menuRotate_triggered(QAction *act)
{
    if (act == &m_actRotateRight)
    {
        m_nRotateDegrees += -90;
        if (m_nRotateDegrees == 360 || m_nRotateDegrees == -360)
        {
            m_nRotateDegrees = 0;
        }
        m_listStepHistory.push_back(STEP_ROTATE_RIGHT);
    }
    else if (act == &m_actRotateLeft)
    {
        m_nRotateDegrees += 90;
        if (m_nRotateDegrees == 360 || m_nRotateDegrees == -360)
        {
            m_nRotateDegrees = 0;
        }
        m_listStepHistory.push_back(STEP_ROTATE_LEFT);
    }

    update();
}

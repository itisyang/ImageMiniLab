#include <QPainter>
#include <QMatrix>
#include <QDebug>
#include <QMetaObject>
#include <QImage>
#include <QFileDialog>
#include <QDir>

#include "ShowWid.h"

//#pragma execution_character_set("utf-8")




ShowWid::ShowWid(QWidget *parent)
    : m_fScale(1),
      xtranslate(0),
      ytranslate(0),
      m_nRotateDegrees(0),
      m_bGrayscale(false),
      m_bMirroredHorizontal(false),
      m_bMirroredVertical(false),
      m_eMousePressStatus(No)
{
    m_listStepHistory.clear();
}

ShowWid::~ShowWid()
{

}


bool ShowWid::Init()
{
    m_actSharpen.setText("锐化");
    m_actDenoise.setText("去噪");
    m_actGrayscale.setText("灰度化");
    m_actGrayscale.setCheckable(true);

    m_actRotateMenu.setText("旋转");
    m_actRotateMenu.setMenu(&m_menuRotate);
    m_actRotateLeft.setText("向左旋转");
    m_actRotateRight.setText("向右旋转");
    m_menuRotate.addAction(&m_actRotateLeft);
    m_menuRotate.addAction(&m_actRotateRight);

    m_actMirroredMenu.setText("镜像");
    m_actMirroredMenu.setMenu(&m_menuMirrored);
    m_actMirroredHorizontal.setText("水平镜像");
    m_actMirroredVertical.setText("垂直镜像");
    m_actMirroredHorizontal.setCheckable(true);
    m_actMirroredVertical.setCheckable(true);
    m_menuMirrored.addAction(&m_actMirroredHorizontal);
    m_menuMirrored.addAction(&m_actMirroredVertical);

    m_actPosRestore.setText("位置还原");

    QList<QAction *> listMenuMouse;
    listMenuMouse << &m_actSharpen << &m_actDenoise << &m_actGrayscale << &m_actRotateMenu << &m_actMirroredMenu << &m_actPosRestore;
    m_menuMouse.addActions(listMenuMouse);


    QList<bool> listRet;

    listRet << connect(&m_actSharpen, &QAction::triggered, this, &ShowWid::on_actSharpen_triggered);
    listRet << connect(&m_actDenoise, &QAction::triggered, this, &ShowWid::on_actDenoise_triggered);
    listRet << connect(&m_actGrayscale, &QAction::triggered, this, &ShowWid::on_actGrayscale_triggered);
    listRet << connect(&m_menuRotate, &QMenu::triggered, this, &ShowWid::On_menuRotate_triggered);
    listRet << connect(&m_menuMirrored, &QMenu::triggered, this, &ShowWid::On_menuMirrored_triggered);
    listRet << connect(&m_actPosRestore, &QAction::triggered, this, &ShowWid::On_actPosRestore_triggered);

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
        QImage stImageTemp = GetShowImage();

        //缩放
        stImageTemp = stImageTemp.scaled(this->width() * m_fScale, this->height() * m_fScale, Qt::KeepAspectRatio);
        //绘制到指定坐标
        painter.drawImage(m_stImageShowPos, stImageTemp);
    }

}


//右键菜单
void ShowWid::contextMenuEvent(QContextMenuEvent *event)
{
    //鼠标右键菜单出现的位置为当前鼠标的位置
    if (m_bGrayscale)
    {
        m_actGrayscale.setChecked(true);
        m_listStepHistory.push_back(STEP_UNGRAYSCALE);
    }
    else
    {
        m_actGrayscale.setChecked(false);
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
        if (m_fScale <= 100)
        {
            m_fScale *= 1.2;
        }
    }
    else if (numDegrees < 0)
    {
        if (m_fScale >= 0.01)
        {
            m_fScale *= 1 / 1.2;
        }
    }

    update();
}

void ShowWid::mouseMoveEvent(QMouseEvent * e)
{
    if (m_eMousePressStatus == Left)
    {
        m_stImageShowPos.setX(m_stImageBeforeMovePos.x() + (e->x() - m_stMousePressPos.x()));
        m_stImageShowPos.setY(m_stImageBeforeMovePos.y() + (e->y() - m_stMousePressPos.y()));

        update();
    }
}

void ShowWid::mousePressEvent(QMouseEvent * e)
{
    switch (e->button())
    {
    case Qt::LeftButton:
        m_eMousePressStatus = Left;
        m_stMousePressPos.setX(e->x());
        m_stMousePressPos.setY(e->y());
        m_stImageBeforeMovePos = m_stImageShowPos;

        break;
    case Qt::RightButton:
        m_eMousePressStatus = Right;
        break;
    case Qt::MiddleButton:
        m_eMousePressStatus = Mid;
        break;
    default:
        m_eMousePressStatus = No;
    }

    m_stMousePressPos.setX(e->x());
    m_stMousePressPos.setY(e->y());
}

void ShowWid::mouseDoubleClickEvent(QMouseEvent *e)
{
    QWidget::mouseDoubleClickEvent(e);
}

//得到显示图像
QImage ShowWid::GetShowImage()
{
    QImage stImageTemp = m_stImage;

    //灰度化
    if (m_bGrayscale && !stImageTemp.allGray())
    {
        QColor oldColor;
        QImage * newImage = new QImage(stImageTemp.width(), stImageTemp.height(), QImage::Format_ARGB32);

        for (int x = 0; x < newImage->width(); x++)
        {
            for (int y = 0; y < newImage->height(); y++)
            {
                //对坐标像素点 红绿蓝 取平均值，三值相等即为灰度图
                oldColor = QColor(stImageTemp.pixel(x, y));
                int average = (oldColor.red() + oldColor.green() + oldColor.blue()) / 3;
                newImage->setPixel(x, y, qRgb(average, average, average));
            }
        }

        stImageTemp = *newImage;

        delete newImage;
    }

    //镜像
    stImageTemp = stImageTemp.mirrored(m_bMirroredHorizontal, m_bMirroredVertical);

    //旋转m_strImagePath
    QMatrix matrix;
    matrix.rotate(m_nRotateDegrees);
    stImageTemp = stImageTemp.transformed(matrix);

    return stImageTemp;
}

void ShowWid::OnSaveAs()
{
    qDebug() << "ShowWid::OnSaveAs";

    QImage stImage = GetShowImage();

    QFileInfo info(m_strImagePath);
    QString strSavePath = QFileDialog::getSaveFileName(this, "另存为", QDir::homePath() + "/" + info.fileName());

    if (!strSavePath.isEmpty())
    {
        stImage.save(strSavePath);
    }
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
        emit SigMessage("打开图片失败");
    }
    else
    {
        m_strImagePath = strImagePath;
        qDebug() << "打开图片成功";
        emit SigMessage("打开图片成功");
    }
    update();
}


void ShowWid::CloseImage()
{
    m_strImagePath.clear();
    m_stImage = QImage();

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

void ShowWid::On_menuMirrored_triggered(QAction *act)
{
    if (act == &m_actMirroredHorizontal)
    {
        m_bMirroredHorizontal = !m_bMirroredHorizontal;
    }
    else if (act == &m_actMirroredVertical)
    {
        m_bMirroredVertical = !m_bMirroredVertical;
    }

    update();
}

void ShowWid::On_actPosRestore_triggered()
{
    m_fScale = 1;
    xtranslate = 0;
    ytranslate = 0;
    m_bMirroredHorizontal = false;
    m_bMirroredVertical = false;
    m_nRotateDegrees = 0;

    update();
}

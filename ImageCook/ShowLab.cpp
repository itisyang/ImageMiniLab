#include <QPainter>
#include <QMatrix>

#include "ShowLab.h"

ShowLab::ShowLab(QWidget *parent)
{
    m_pixBase;
}

ShowLab::~ShowLab()
{

}

void ShowLab::paintEvent(QPaintEvent *event)
{
    QPainter painter;

    if (m_pixBase.width() && m_pixBase.height())
    {
        QPixmap pix = m_pixBase.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        painter.begin(this);
        painter.drawPixmap(0, 0, pix.width(), pix.height(), pix);
    }
    
}

void ShowLab::On_SetBasePix(QPixmap pix)
{
    m_pixBase = pix;
}

void ShowLab::On_SetRotate(int nDegrees)
{
    QMatrix matrix;
    matrix.rotate(nDegrees);
    m_pixBase = m_pixBase.transformed(matrix,Qt::SmoothTransformation);
    update();
}

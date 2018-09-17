#include <QFileDialog>
#include <QDebug>
#include <QPainter>
#include <QAction>
#include <QFile>

#include "imageminilab.h"

#pragma execution_character_set("utf-8")

ImageMiniLab::ImageMiniLab(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    QList<bool> listRet;
    listRet << connect(this, SIGNAL(Sig_BasePix(QPixmap)), ui.ShowWidget, SLOT(On_SetBasePix(QPixmap)));
    listRet << connect(this, SIGNAL(Sig_Undo()), ui.ShowWidget, SLOT(On_Undo()));

    listRet << connect(this, &ImageMiniLab::SigOpenImage, ui.ShowWidget, &ShowWid::OpenImage);

    for (bool ret : listRet)
    {
        if (!ret)
        {
            qDebug() << "初始化失败";
        }
    }
}

void ImageMiniLab::on_ActOpen_triggered()
{
    qDebug() << "on_actionopen_triggered";
    m_ImagePath = QFileDialog::getOpenFileName(this, tr("Open File"),
        ".",
        tr("Images (*.png *.xpm *.jpg)"));
    if (!m_ImagePath.isEmpty())
    {
        emit SigOpenImage(m_ImagePath);
    }
}

void ImageMiniLab::on_ActClose_triggered()
{
    qDebug() << "on_actionclose_triggered";
    m_ImagePath.clear();
}

void ImageMiniLab::on_ActSave_triggered()
{
    if (m_ImagePath.isEmpty())
    {
        ui.StatusBar->showMessage("未打开文件", 3000);

        return;
    }
}

void ImageMiniLab::on_ActSaveAs_triggered()
{
    if (m_ImagePath.isEmpty())
    {
        ui.StatusBar->showMessage("未打开文件", 3000);

        return;
    }
}

void ImageMiniLab::on_ActUndo_triggered()
{
    emit Sig_Undo();
}

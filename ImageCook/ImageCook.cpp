#include "ImageCook.h"

#include <QFileDialog>
#include <QDebug>
#include <QPainter>

ImageCook::ImageCook(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	
    //旋转菜单
//    QAction* RotateRight90 = new QAction();
//    RotateRight90->setText("向右旋转90度");
//    QAction* RotateLeft90 = new QAction();
//    RotateLeft90->setText("向左旋转90度");
//    QMenu *pRotateMenu = new QMenu();
//    pRotateMenu->addAction(RotateRight90);
//    pRotateMenu->addAction(RotateLeft90);
//    connect(pRotateMenu, SIGNAL(triggered(QAction*)), this, SLOT(on_ActRotate(QAction*)));
//    ui.BtnRotate->setMenu(pRotateMenu);

    ui.LabShow->clear();
}

void ImageCook::on_ActOpen_triggered()
{
	qDebug() << "on_actionopen_triggered";
    m_ImagePath = QFileDialog::getOpenFileName(this, tr("Open File"),
        "/home",
		tr("Images (*.png *.xpm *.jpg)"));
    if (!m_ImagePath.isEmpty())
	{
        m_Pix.load(m_ImagePath);
        ui.LabShow->setPixmap(m_Pix);
	}
}

void ImageCook::on_ActClose_triggered()
{
	qDebug() << "on_actionclose_triggered";
    ui.LabShow->clear();
    m_ImagePath.clear();
}

void ImageCook::on_ActSave_triggered()
{
    if (m_ImagePath.isEmpty())
    {
        ui.StatusBar->showMessage("未打开文件", 3000);

        return;
    }


}

void ImageCook::on_ActSaveAs_triggered()
{
    if (m_ImagePath.isEmpty())
    {
        ui.StatusBar->showMessage("未打开文件", 3000);

        return;
    }
}

void ImageCook::on_ActRotate(QAction *act)
{
    qDebug() << act->text();
//    if (act->text() == "向左旋转90度")
//    {
//        QPainter painter(&m_Pix);
//        QPixmap pix = m_Pix;
//        painter.rotate(90);
//        painter.drawPixmap(0, 0, pix);
//        ui.LabShow->clear();
//        ui.LabShow->setPixmap(m_Pix);


//    }
//    else if (act->text() == "向右旋转90度")
//    {

//    }

}


#include "ImageCook.h"

#include <QFileDialog>
#include <QDebug>

ImageCook::ImageCook(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	
	ui.labelshow->clear();
}

void ImageCook::on_actionopen_triggered()
{
	qDebug() << "on_actionopen_triggered";
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
		"/home",
		tr("Images (*.png *.xpm *.jpg)"));
	if (!fileName.isEmpty())
	{
		QPixmap pix;
		pix.load(fileName);
		ui.labelshow->setPixmap(pix);
	}
}

void ImageCook::on_actionclose_triggered()
{
	qDebug() << "on_actionclose_triggered";
	ui.labelshow->clear();
}


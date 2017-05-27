#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ImageCook.h"

class ImageCook : public QMainWindow
{
	Q_OBJECT

public:
	ImageCook(QWidget *parent = Q_NULLPTR);

private:
	Ui::ImageCookClass ui;
};

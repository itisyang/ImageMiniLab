#include "ImageCook.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ImageCook w;
	w.show();
	return a.exec();
}

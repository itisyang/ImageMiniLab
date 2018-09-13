#include "imageminilab.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ImageMiniLab w;
    w.show();
    return a.exec();
}

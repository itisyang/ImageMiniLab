#include "imageminilab.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ImageMiniLab w;
    if (!w.Init())
    {
        return -1;
    }
    w.show();
    return a.exec();
}

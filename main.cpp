#include "MainWindow.h"

#include <QApplication>
#include "GlobalData.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    globalPix = new QPixmap();
    MainWindow w;
    w.show();
    return a.exec();
}

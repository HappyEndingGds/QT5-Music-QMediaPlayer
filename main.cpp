#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("音乐播放器");
    //w.setWindowOpacity(0.7);
    w.show();

    return a.exec();
}

#include "mainwindow.h"

#include <QApplication>
#include <QGuiApplication>
#include <QScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //w.resize(1920, 1080);
    //w.resize(QGuiApplication::primaryScreen()->availableGeometry().size() * 1);
    w.setWindowState(Qt::WindowMaximized);
    w.show();
    return a.exec();
}

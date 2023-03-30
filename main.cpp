/*
 * Jason Gill 21304092
 *
 * GitHub repo: https://github.com/jasonjayo/recipes
 *
 * Recipes sourced from https://bbcgoodfood.com/
 * Images from https://pixabay.com/
 * Icons from https://fonts.google.com/icons
 */

#include "mainwindow.h"

#include <QApplication>
#include <QGuiApplication>
#include <QScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowState(Qt::WindowMaximized);
    w.show();
    return a.exec();
}

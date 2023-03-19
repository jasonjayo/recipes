#include "mainwindow.h"
#include "qpushbutton.h"
#include "qjsondocument.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QLabel>
#include <QTextEdit>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPushButton* btn = new QPushButton("A button");
    ui->gridLayout->addWidget(btn);

    // TODO: remove absolute file name
    QFile recipesFile("C:/Users/jason/Desktop/College/Modules/CS4076/Project/recipes/recipes.json");
    recipesFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QString data = recipesFile.readAll();

    QJsonDocument json = QJsonDocument::fromJson(data.toUtf8());
    QJsonArray recipes = json.array();

    QTextFormat* titleFormat = new QTextFormat();
    titleFormat->setProperty(QTextFormat::FontPixelSize, 12);

    // for each recipe
    for (const QJsonValue &recipe : recipes) {
        //qDebug() << recipe["title"].toString();
        btn->setText(recipe["title"].toString());
        QLabel* title = new QLabel(recipe["title"].toString());
        ui->gridLayout->addWidget(title);
        title->setStyleSheet("QLabel {font-size: 20px}");

    }

    recipesFile.close();

}

MainWindow::~MainWindow()
{
    delete ui;
}


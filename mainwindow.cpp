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
#include <QGuiApplication>
#include <QScreen>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    //resize(QGuiApplication::primaryScreen()->availableGeometry().size() * 1);
    //qDebug() << QGuiApplication::primaryScreen()->availableGeometry().size();

    //QPushButton* btn = new QPushButton("A button");
    //ui->gridLayout->addWidget(btn);
    //ui->centralwidget->setMinimumWidth(1200);


    // TODO: remove absolute file name
    QFile recipesFile("C:/Users/jason/Desktop/College/Modules/CS4076/Project/recipes/recipes.json");
    recipesFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QString data = recipesFile.readAll();

    QJsonDocument json = QJsonDocument::fromJson(data.toUtf8());
    QJsonArray recipes = json.array();

    QTextFormat* titleFormat = new QTextFormat();
    titleFormat->setProperty(QTextFormat::FontPixelSize, 12);

    ui->gridLayout->setColumnMinimumWidth(0, 50);



    int i = 0;
    // for each recipe
    for (const QJsonValue &recipe : recipes) {
        QVBoxLayout* recipeContainer = new QVBoxLayout();

        QLabel* image = new QLabel();

        QJsonArray photos = recipe["photos"].toArray();
        QPixmap pix("C:/Users/jason/Desktop/College/Modules/CS4076/Project/recipes/" + photos.first().toString());
        //image->setPixmap(pix.scaled( image->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        image->setPixmap(pix.scaled(300, 200));
        //image->setScaledContents(true);
        recipeContainer->addWidget(image);

        QLabel* title = new QLabel(recipe["title"].toString());
        title->setStyleSheet("QLabel {font-size: 26px; background: #fff; font-weight:bold; padding: 6px;}");
        title->setWordWrap(true);
        recipeContainer->addWidget(title);

        // stats
        QGridLayout* statsGrid = new QGridLayout;
        QJsonObject stats = recipe["stats"].toObject();
        QJsonObject stats_time = stats["times"].toObject();


        qDebug() << stats_time.value("prep").toDouble();

        QString str(QString::number(stats_time.value("prep").toDouble()));


        QLabel* timeIconLabel = new QLabel;
        QLabel* timeValue = new QLabel(QString("Prep in " + str + " minutes"));
        QPixmap timeIcon(":/images/icon_clock.svg");
        timeValue->setStyleSheet("background: #fff;");
        timeIconLabel->setPixmap(timeIcon);
        statsGrid->addWidget(timeIconLabel, 0, 0);
        statsGrid->addWidget(timeValue, 0, 1, 1, 4);
        recipeContainer->addLayout(statsGrid);

        QLabel* servingCountIconLabel = new QLabel;
        QLabel* servingCountValue = new QLabel("Serves 4");
        QPixmap servingCountIcon(":/images/icon_restaurant.svg");
        servingCountValue->setStyleSheet("background: #fff;");
        servingCountIconLabel->setPixmap(servingCountIcon);
        statsGrid->addWidget(servingCountIconLabel, 1, 0);
        statsGrid->addWidget(servingCountValue, 1, 1, 1, 4);
        recipeContainer->addLayout(statsGrid);

        QLabel* description = new QLabel(recipe["description"].toString());
        description->setWordWrap(true);
        description->setStyleSheet("background:#fff; padding: 6px;border: 4px solid #cfcfcf");
        recipeContainer->addWidget(description);

        QPushButton* viewBtn = new QPushButton;
        viewBtn->setText("View recipe");
        viewBtn->setStyleSheet("QPushButton { background: #cfcfcf; border: none; padding: 8px 0; font-weight:bold; text-transform:uppercase; font-size: 12px; }QPushButton:hover {background: #fff}");
        recipeContainer->addWidget(viewBtn);



        ui->gridLayout->addLayout(recipeContainer, 0, i);
        i++;


    }

    recipesFile.close();

}

MainWindow::~MainWindow()
{
    delete ui;
}


#include "mainwindow.h"
#include "qpushbutton.h"
#include "qjsondocument.h"
#include "ui_mainwindow.h"
#include "missingfileexception.h"

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
#include <Recipe.h>
#include <QDir>
#include <QLayout>
#include <iostream>

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
    if (!recipesFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw new MissingFileException("recipes.json");
    }
    QString data = recipesFile.readAll();

    QJsonDocument json = QJsonDocument::fromJson(data.toUtf8());
    QJsonArray recipes = json.array();

    QTextFormat* titleFormat = new QTextFormat();
    titleFormat->setProperty(QTextFormat::FontPixelSize, 12);

    ui->gridLayout->setColumnMinimumWidth(0, 50);





    //    QFile file(QDir::homePath() + "/something.json");
    // C:/Users/jason

    int i = 0;

    // for each recipe
    for (const QJsonValue &recipe : recipes) {
        QVBoxLayout* recipeContainer = new QVBoxLayout();

        recipeContainer->setSpacing(10);

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

        QString str(QString::number(stats_time.value("prep").toDouble()));


        QLabel* timeIconLabel = new QLabel;
        QLabel* timeValue = new QLabel(QString("Prep in " + str + " minutes"));
        QPixmap timeIcon(":/images/icon_clock.svg");
        timeValue->setStyleSheet("background: #fff;");
        timeIconLabel->setPixmap(timeIcon);
        statsGrid->addWidget(timeIconLabel, 0, 0);
        statsGrid->addWidget(timeValue, 0, 1, 1, 4);

        QLabel* servingCountIconLabel = new QLabel;
        int servingCountValue = stats["serves"].toInt();
        QPixmap servingCountIcon(":/images/icon_restaurant.svg");
        QLabel* servingCountValueLabel = new QLabel(QString(QString::number(servingCountValue)));
        servingCountValueLabel->setStyleSheet("background: #fff;");
        servingCountIconLabel->setPixmap(servingCountIcon);
        statsGrid->addWidget(servingCountIconLabel, 1, 0);
        statsGrid->addWidget(servingCountValueLabel, 1, 1, 1, 4);

        recipeContainer->addLayout(statsGrid);

        QLabel* description = new QLabel(recipe["description"].toString());
        description->setWordWrap(true);
        description->setStyleSheet("background:#fff; padding: 6px;border: 4px solid #cfcfcf");
        recipeContainer->addWidget(description);

        QPushButton* viewBtn = new QPushButton;
        viewBtn->setText("View recipe");
        viewBtn->setStyleSheet("QPushButton { background: #cfcfcf; border: none; padding: 8px 0; font-weight:bold; text-transform:uppercase; font-size: 12px; }QPushButton:hover {background: #fff}");
        recipeContainer->addWidget(viewBtn);


        //        Recipe* r = new Recipe();

        RecipeStats recipeStats;
        recipeStats.cookTime = stats_time.value("cook").toInt();
        recipeStats.prepTime = stats_time.value("prep").toInt();
        recipeStats.difficulty = 0;
        recipeStats.serves = servingCountValue;

        Nutrition nutrition;

        QJsonArray ingredientsJsonArray = recipe["ingredients"].toArray();

        QList<Ingredient> ingredients;
        for (const QJsonValue &ingredientVal: ingredientsJsonArray) {
            QJsonObject ingredientObj = ingredientVal.toObject();
            Ingredient ingredient(ingredientObj["name"].toString(), ingredientObj["quantity"].toString());
            ingredients.push_back(ingredient);
        }

        QList<QString> instructions;
        Recipe* recipeObj = new Recipe(recipe["title"].toString(), recipe["descriptipn"].toString(), recipeStats, ingredients, nutrition, false, false, instructions);


        ui->gridLayout->addLayout(recipeContainer, i / 4, i % 4);
        ui->gridLayout->setRowMinimumHeight(i / 4, 600);
        i++;


    }

    recipesFile.close();

    //    ui->stackedWidget->setCurrentIndex(1);
    //    setCentralWidget(ui->stackedWidget); // :D
    setCentralWidget(ui->scrollArea);

}

//void createRecipeCard(Recipe r, QGridLayout l) {

//}


MainWindow::~MainWindow()
{
    delete ui;
}


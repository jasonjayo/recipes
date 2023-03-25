#include "mainwindow.h"
#include "qjsondocument.h"
#include "ui_mainwindow.h"
#include "missingfileexception.h"
#include "foodrecipe.h"
#include "drinkrecipe.h"

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
#include <recipe.h>
#include <QDir>
#include <QLayout>
#include <iostream>

#define NUM_CARDS_PER_LINE 4
#define GRID_TOP_OFFSET 1

// global var
QList<FoodRecipe*> foodRecipes;
QList<DrinkRecipe*> drinkRecipes;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

#ifdef GRID_TOP_OFFSET

    QLabel* topRow = new QLabel("Compiled " + QString(__DATE__) + " " + QString(__TIME__));
    ui->gridLayout->addWidget(topRow, 0, 0, GRID_TOP_OFFSET, NUM_CARDS_PER_LINE);
    //resize(QGuiApplication::primaryScreen()->availableGeometry().size() * 1);
    //qDebug() << QGuiApplication::primaryScreen()->availableGeometry().size();
#endif
    //    QFile file(QDir::homePath() + "/something.json");
    // C:/Users/jason

    // TODO: remove absolute file name
    QFile recipesFile("C:/Users/jason/Desktop/College/Modules/CS4076/Project/recipes/recipes.json");
    if (!recipesFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw new MissingFileException("recipes.json");
    }
    QString data = recipesFile.readAll();

    QJsonDocument json = QJsonDocument::fromJson(data.toUtf8());
    QJsonArray recipesJsonArr = json.array();

    QTextFormat* titleFormat = new QTextFormat();
    titleFormat->setProperty(QTextFormat::FontPixelSize, 12);

    //ui->gridLayout->setColumnMinimumWidth(0, 50);



    int i = 0;

    // for each recipe
    for (const QJsonValue &recipeJsonObj : recipesJsonArr) {

        // get all values ready in prep for creating object
        QString title = recipeJsonObj["title"].toString();
        QString recipeType = recipeJsonObj["type"].toString();
        QString description = recipeJsonObj["description"].toString();


        QJsonArray photosJsonArr = recipeJsonObj["photos"].toArray();
        QList<QString> photos;
        for (const QJsonValue &photoVal: photosJsonArr) {
            photos.push_back(photoVal.toString());
        }

        QJsonObject stats = recipeJsonObj["stats"].toObject();
        QJsonObject stats_time = stats["times"].toObject();
        int stats_time_prep(stats_time.value("prep").toInt());
        int stats_time_cook(stats_time.value("cook").toInt());
        int stats_difficulty(stats_time.value("difficulty").toInt());

        RecipeStats recipeStats;
        recipeStats.cookTime = stats_time_cook;
        recipeStats.prepTime = stats_time_prep;
        recipeStats.difficulty = stats_difficulty;

        QList<Ingredient> ingredients;
        QJsonArray ingredientsJsonArray = recipeJsonObj["ingredients"].toArray();
        for (const QJsonValue &ingredientVal: ingredientsJsonArray) {
            QJsonObject ingredientObj = ingredientVal.toObject();
            ingredients.push_back(Ingredient(ingredientObj["name"].toString(), ingredientObj["quantity"].toString()));
        }

        Nutrition nutrition;
        QJsonObject nutritionJsonObj = recipeJsonObj["nutrition"].toObject();
        nutrition.kcal = nutritionJsonObj["kcal"].toDouble();
        nutrition.fat = nutritionJsonObj["fat"].toDouble();
        nutrition.saturates = nutritionJsonObj["saturates"].toDouble();
        nutrition.carbs = nutritionJsonObj["carbs"].toDouble();
        nutrition.sugars = nutritionJsonObj["sugars"].toDouble();
        nutrition.fibre = nutritionJsonObj["fibre"].toDouble();
        nutrition.protein = nutritionJsonObj["protein"].toDouble();
        nutrition.salt = nutritionJsonObj["salt"].toDouble();

        QList<QString> allergens;
        QJsonArray allergensJsonArr = recipeJsonObj["allergens"].toArray();
        for (const QJsonValue &allergenVal: allergensJsonArr) {
            allergens.push_back(allergenVal.toString());
        }

        bool vegan = recipeJsonObj["vegan"].toBool();
        bool vegetarian = recipeJsonObj["vegetarian"].toBool();

        QList<QString> instructions;
        QJsonArray instructionsJsonArr = recipeJsonObj["instructions"].toArray();
        for (const QJsonValue &instructionVal: instructionsJsonArr) {
            instructions.push_back(instructionVal.toString());
        }

        Recipe* r;
        if (recipeType == "food") {
            FoodRecipe* recipeObj = new FoodRecipe(title, description, photos, recipeStats, ingredients, nutrition, vegan, vegetarian, instructions, recipeJsonObj["serves"].toInt());
            foodRecipes.push_back(recipeObj);
            r = recipeObj;
        } else {
            DrinkRecipe* recipeObj = new DrinkRecipe(title, description, photos, recipeStats, ingredients, nutrition, vegan, vegetarian, instructions, false);
            drinkRecipes.push_back(recipeObj);
            r = recipeObj;
        }

        //        qDebug() << static_cast<FoodRecipe*>(recipeObj)->getServings();
        //        recipeContainer

        QVBoxLayout* recipeContainer = r->createCard();
        qDebug() << "putting " << (i / NUM_CARDS_PER_LINE) + GRID_TOP_OFFSET << "\n";
        ui->gridLayout->addLayout(recipeContainer, (i / NUM_CARDS_PER_LINE) + GRID_TOP_OFFSET, i % 4);
        ui->gridLayout->setRowMinimumHeight((i / NUM_CARDS_PER_LINE) + GRID_TOP_OFFSET, 600);
        i++;

    }



    recipesFile.close();

    //    ui->stackedWidget->setCurrentIndex(1);
    setCentralWidget(ui->scrollArea);

}

MainWindow::~MainWindow()
{
    delete ui;
}


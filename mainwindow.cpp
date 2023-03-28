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
#include <QGroupBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QContextMenuEvent>
#include <QAction>
#include <QSlider>

#define NUM_CARDS_PER_LINE 4
#define GRID_TOP_OFFSET 1

// global var
QList<FoodRecipe*> foodRecipes;
QList<DrinkRecipe*> drinkRecipes;

bool showFoodOnly = false;
bool showDrinksOnly = false;

bool showVeganOnly = false;
bool showVegetarianOnly = false;

int maxTime = 200;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    resetAct = new QAction("Reset list", this);
    resetAct->setStatusTip("Reset the list of displayed recipes");
    connect(resetAct, &QAction::triggered, this, &MainWindow::removeAllCards);

#ifdef GRID_TOP_OFFSET

    QLabel* topRow = new QLabel("Compiled " + QString(__DATE__) + " " + QString(__TIME__));
    ui->gridLayout->addWidget(topRow, 0, 0);
    //resize(QGuiApplication::primaryScreen()->availableGeometry().size() * 1);
    //qDebug() << QGuiApplication::primaryScreen()->availableGeometry().size();
#endif
    //    QFile file(QDir::homePath() + "/something.json");
    // C:/Users/jason

    QGroupBox* filtersBox = new QGroupBox("Filters");
    QCheckBox* veganFilterBox = new QCheckBox("Vegan");
    QCheckBox* vegetarianFilterBox = new QCheckBox("Vegetarian");
    QVBoxLayout* filtersVBox = new QVBoxLayout();
    QRadioButton* showFoodOnlyFilterBtn = new QRadioButton("Show food only");
    QRadioButton* showDrinksOnlyFilterBtn = new QRadioButton("Show drinks only");
    QRadioButton* showAllFilterBtn = new QRadioButton("Show all");
    QLabel* timeSliderLabel = new QLabel("Prep + cook time max.");
    QLabel* timeSliderValue = new QLabel(QString::number(maxTime) + " mins.");
    QHBoxLayout* timeSliderContainer = new QHBoxLayout();
    QSlider* servingsSlider = new QSlider(Qt::Horizontal);
    servingsSlider->setMaximumWidth(150);
    timeSliderContainer->addWidget(timeSliderLabel);
    timeSliderContainer->addWidget(servingsSlider, Qt::AlignLeft);
    timeSliderContainer->addWidget(timeSliderValue);
    timeSliderContainer->addStretch(1);
    connect(servingsSlider, &QSlider::sliderMoved, this, [=](int pos) {
         maxTime = pos;
         timeSliderValue->setText(QString::number(pos) + " mins.");
         displayCards();
    });
    filtersVBox->addWidget(veganFilterBox);
    filtersVBox->addWidget(vegetarianFilterBox);
    filtersVBox->addWidget(showFoodOnlyFilterBtn);
    filtersVBox->addWidget(showDrinksOnlyFilterBtn);
    filtersVBox->addWidget(showAllFilterBtn);
    filtersVBox->addLayout(timeSliderContainer);
    filtersBox->setLayout(filtersVBox);

    QPushButton* removeAllCardsBtn = new QPushButton("Remove all cards");
    filtersVBox->addWidget(removeAllCardsBtn);
    connect(removeAllCardsBtn, &QPushButton::clicked, this, &MainWindow::removeAllCards);

    showAllFilterBtn->setChecked(true);
    servingsSlider->setTickInterval(1);
    servingsSlider->setMinimum(5);
    servingsSlider->setMaximum(120);

    ui->gridLayout->addWidget(filtersBox, 0, 1, 1, 3);


    veganFilterBox->setChecked(true);
    connect(veganFilterBox, &QCheckBox::stateChanged, this, &MainWindow::displayCards);
    connect(veganFilterBox, &QCheckBox::stateChanged, this, [=]() {
        qDebug() << "setting showVeganOnly to " << veganFilterBox->isChecked();
        showVeganOnly = veganFilterBox->isChecked();
        displayCards();
    });

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

        DietaryInfo diateryInfo;
        QJsonObject diateryInfoJsonObj = recipeJsonObj["dietaryInfo"].toObject();
        diateryInfo.vegan = diateryInfoJsonObj["vegan"].toBool();
        diateryInfo.vegetarian = diateryInfoJsonObj["vegetarian"].toBool();
        diateryInfo.glutenFree = diateryInfoJsonObj["glutenFree"].toBool();
        diateryInfo.lactoseFree = diateryInfoJsonObj["lactoseFree"].toBool();

        qDebug() << diateryInfo.vegan;

        QList<QString> instructions;
        QJsonArray instructionsJsonArr = recipeJsonObj["instructions"].toArray();
        for (const QJsonValue &instructionVal: instructionsJsonArr) {
            instructions.push_back(instructionVal.toString());
        }

        //        Recipe* r;
        if (recipeType == "food") {
            FoodRecipe* recipeObj = new FoodRecipe(title, description, photos, recipeStats, ingredients, nutrition, vegan, vegetarian, instructions, diateryInfo, recipeJsonObj["serves"].toInt());
            foodRecipes.push_back(recipeObj);
            //            r = recipeObj;
        } else {
            DrinkRecipe* recipeObj = new DrinkRecipe(title, description, photos, recipeStats, ingredients, nutrition, vegan, vegetarian, instructions, diateryInfo, false);
            drinkRecipes.push_back(recipeObj);
            //            r = recipeObj;
        }

        //        qDebug() << static_cast<FoodRecipe*>(recipeObj)->getServings();
        //        recipeContainer

        //QVBoxLayout* recipeContainer = r->createCard();
        //ui->gridLayout->addLayout(recipeContainer, (i / NUM_CARDS_PER_LINE) + GRID_TOP_OFFSET, i % 4);
        //ui->gridLayout->setRowMinimumHeight((i / NUM_CARDS_PER_LINE) + GRID_TOP_OFFSET, 600);
        //        i++;



    }

    displayCards();

    // working on copy constructor
//    qDebug() << &drinkRecipes.at(0)->instructions;
//    DrinkRecipe copy = *drinkRecipes.at(0);
//    qDebug() << &copy.instructions;

    std::ostringstream buffer;
    buffer << drinkRecipes.at(0);
    std::string s = buffer.str();
    QString str = QString::fromStdString(s);
    qDebug() << str;

    DrinkRecipe dr = *drinkRecipes.at(0);
    std::cout << dr;

//qDebug() << *drinkRecipes.at(0) + *drinkRecipes.at(1);


    recipesFile.close();

    //    ui->stackedWidget->setCurrentIndex(1);
    setCentralWidget(ui->scrollArea);

}

void MainWindow::displayCards() {
    //qDebug() << "showing vegan only? " << showVeganOnly;
    removeAllCards();

//    qDebug() << ui->gridLayout->count();
//    for (int i = 2; i < ui->gridLayout->count(); i++) {
//        QLayoutItem* l = ui->gridLayout->itemAt(i)->layout();
//        qDebug() << "removing " << l;
//        ui->gridLayout->removeItem(l);
//    }

    int i = 0;
    if (!showDrinksOnly) {
        for (FoodRecipe* &r: foodRecipes) {
            if (showVeganOnly && !(r->vegan)) continue;
            if (showVegetarianOnly && !(r->vegetarian)) continue;
            if (maxTime < (r->stats.prepTime + r->stats.cookTime)) continue;

            QVBoxLayout* recipeContainer = r->createCard();
            ui->gridLayout->addLayout(recipeContainer, (i / NUM_CARDS_PER_LINE) + GRID_TOP_OFFSET, i % 4);
//            ui->gridLayout->setRowMinimumHeight((i / NUM_CARDS_PER_LINE) + GRID_TOP_OFFSET, 600);
            i++;
        }
    }
    if (!showFoodOnly) {
        for (DrinkRecipe* &r: drinkRecipes) {
            if (showVeganOnly && !(r->vegan)) continue;
            if (showVegetarianOnly && !(r->vegetarian)) continue;
            if (maxTime < (r->stats.prepTime + r->stats.cookTime)) continue;

            QVBoxLayout* recipeContainer = r->createCard();
            ui->gridLayout->addLayout(recipeContainer, (i / NUM_CARDS_PER_LINE) + GRID_TOP_OFFSET, i % 4);
//            ui->gridLayout->setRowMinimumHeight((i / NUM_CARDS_PER_LINE) + GRID_TOP_OFFSET, 600);
            i++;
        }
    }
}

void MainWindow::removeAllCards() {
    qDebug() << "grid count: " << ui->gridLayout->count();

//    QVBoxLayout *child;
//    while ((child = static_cast<QVBoxLayout*>(ui->gridLayout->takeAt(2))) != nullptr) {
//        qDebug() << "removing";
////        delete child->widget(); // delete the widget
////        delete child;   // delete the layout item=
////        child->deleteLater();

//    }
//    delete ui->gridLayout->itemAt(0);
//    ui->gridLayout->update();
//    for (int i = 2; i < ui->gridLayout->count(); i++) {
//        ui->gridLayout->takeAt(i);
//        QLayoutItem* l = ui->gridLayout->itemAt(i)->layout();
//        qDebug() << "removing " << l;
//       ui->gridLayout->removeItem(l);
//      delete l;
//       ui->gridLayout->update();
//    }

    for (int i = 2; i < ui->gridLayout->count(); i++) {
//        QLayout* layout = ui->gridLayout->itemAt(i)->layout();
        clearWidgets(ui->gridLayout, 2);
//        for (int k = 0; k < layout->count(); k++) {
//            layout->takeAt(k);
//        }
    }
    qDebug() << "grid count after: " << ui->gridLayout->count();
}

void MainWindow::clearWidgets(QLayout * layout, int startIndex) {
   if (!layout) return;
   while (auto item = layout->takeAt(startIndex)) {
      delete item->widget();
      clearWidgets(item->layout(), 0);
   }
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event) {
    QMenu menu(this);
    menu.addAction(resetAct);
    menu.exec(event->globalPos());
}

MainWindow::~MainWindow()
{
    delete ui;
}


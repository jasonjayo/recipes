#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "missingfileexception.h"
#include "foodrecipe.h"
#include "drinkrecipe.h"
#include "utils.h"
#include <recipe.h>
#include <typeinfo>

#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>
#include <QLabel>
#include <QTextEdit>
#include <QGuiApplication>
#include <QScreen>
#include <QString>
#include <QDir>
#include <QLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QContextMenuEvent>
#include <QAction>
#include <QSlider>

#define NUM_CARDS_PER_LINE 4
#define GRID_TOP_OFFSET 1

// global vars
QList<FoodRecipe*> foodRecipes;
QList<DrinkRecipe*> drinkRecipes;

std::string foodFacts[] = {
    "Leafy greens like spinach and kale are high in vitamins and minerals and help to improve heart health.",
    "Fatty fish like salmon are rich in omega-3, which helps improve brain function.",
    "Greek yogurt helps to promote gut health.",
    "Green tea has anti-inflammatory properties!"
};

int foodFactsLength = sizeof(foodFacts) / sizeof(std::string);

bool showFoodOnly = false;
bool showDrinksOnly = false;

bool showVeganOnly = false;
bool showVegetarianOnly = false;

int maxTime = 100;

// vars for recipe details page
QGridLayout* rd_grid;
QLabel* rd_title;
QLabel* rd_description;
QLabel* rd_image;
QLabel* rd_stats;
QLabel* rd_ingredients;
QLabel* rd_nutrition;
QHBoxLayout* rd_diateryInfo;
QLabel* rd_instructions;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // QMenu right click context menu
    resetAct = new QAction("Reset list", this);
    resetAct->setStatusTip("Reset the list filters to their initial state");

    // arrays and pointers
    srand(time(0));
    int ranNum = rand();
    std::string fact = *(foodFacts + (ranNum % foodFactsLength)); // pointer arithmetic



    /*
     * BEGIN HOME PAGE UI SET UP
     */

    // advanced preprocessor directives & build-in macros
#ifdef GRID_TOP_OFFSET
    QLabel* topRow = new QLabel("<h1 style='text-transform:uppercase'>Recipe Discovery App</h1><i>Jason Gill (21304092)</i><br><br>Compiled " + QString(__DATE__) + " " + QString(__TIME__) + " C++ " + QString::number(__cplusplus) + "<br><br>" + "<b>Did you know?</b> " + QString::fromStdString(fact));
    topRow->setTextFormat(Qt::RichText); // allows html formatting
    topRow->setWordWrap(true);
    ui->gridLayout->addWidget(topRow, 0, 0);
#else
    QLabel* topRow = new QLabel("Something is wrong!");
    ui->gridLayout->addWidget(topRow, 0, 0);
#endif

    // set up filters box
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
    QSlider* timeSlider = new QSlider(Qt::Horizontal);

    timeSlider->setMaximumWidth(150);
    timeSliderContainer->addWidget(timeSliderLabel);
    timeSliderContainer->addWidget(timeSlider, Qt::AlignLeft);
    timeSliderContainer->addWidget(timeSliderValue);
    timeSliderContainer->addStretch(1);
    connect(timeSlider, &QSlider::sliderMoved, this, [=](int pos) {
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

    showAllFilterBtn->setChecked(true);
    timeSlider->setTickInterval(1);
    timeSlider->setMinimum(5);
    timeSlider->setMaximum(120);
    timeSlider->setSliderPosition(maxTime);

    filtersBox->setMaximumHeight(250);
    filtersBox->setAlignment(Qt::AlignTop);

    ui->gridLayout->addWidget(filtersBox, 0, 1, 1, 3);


    veganFilterBox->setChecked(showVeganOnly);
    vegetarianFilterBox->setChecked(showVegetarianOnly);

    // using anonymous functions / lambda expressions for convenience
    connect(veganFilterBox, &QCheckBox::stateChanged, this, [=]() {
        showVeganOnly = veganFilterBox->isChecked();
        displayCards();
    });
    connect(vegetarianFilterBox, &QCheckBox::stateChanged, this, [=]() {
        showVegetarianOnly = vegetarianFilterBox->isChecked();
        displayCards();
    });
    connect(showFoodOnlyFilterBtn, &QRadioButton::clicked, this, [=]() {
        showFoodOnly = true;
        showDrinksOnly = false;
        displayCards();
    });
    connect(showDrinksOnlyFilterBtn, &QRadioButton::clicked, this, [=]() {
        showFoodOnly = false;
        showDrinksOnly = true;
        displayCards();
    });
    connect(showAllFilterBtn, &QRadioButton::clicked, this, [=]() {
        showFoodOnly = false;
        showDrinksOnly = false;
        displayCards();
    });
    // for right click context menu item
    connect(resetAct, &QAction::triggered, this, [=]() {
        showFoodOnly = showDrinksOnly = false;
        showVeganOnly = showVegetarianOnly = false;
        veganFilterBox->setChecked(false);
        vegetarianFilterBox->setChecked(false);
        showFoodOnlyFilterBtn->setChecked(false);
        showDrinksOnlyFilterBtn->setChecked(false);
        showAllFilterBtn->setChecked(true);
        timeSlider->setValue(100);
        timeSliderValue->setText("100 mins.");
        maxTime = 100;
        displayCards();
    });

    QPushButton* backBtn = new QPushButton("< Back");
    //    backBtn->setMaximumWidth(100);
    connect(backBtn, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(0);
    });
    ui->gridLayout_p2->addWidget(backBtn, 0, 0);

    /*
     * END HOME PAGE UI SET UP
     */



    /*
     * START JSON READING
     */

    // file brought in as a resource
    QFile recipesFile(":/recipes/recipes.json");
    if (!recipesFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // throw the custom exception if file missing
        throw new MissingFileException("recipes.json");
    }

    QString data = recipesFile.readAll();
    QJsonDocument json = QJsonDocument::fromJson(data.toUtf8());
    QJsonArray recipesJsonArr = json.array();


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
        int stats_difficulty(stats.value("difficulty").toInt());


        RecipeStats recipeStats;
        recipeStats.cookTime = stats_time_cook;
        recipeStats.prepTime = stats_time_prep;
        Difficulty d;
        d.numericalVal = stats_difficulty;
        recipeStats.difficulty = d;

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

        DietaryInfo diateryInfo;
        QJsonObject diateryInfoJsonObj = recipeJsonObj["dietaryInfo"].toObject();
        diateryInfo.vegan = diateryInfoJsonObj["vegan"].toBool();
        diateryInfo.vegetarian = diateryInfoJsonObj["vegetarian"].toBool();
        diateryInfo.glutenFree = diateryInfoJsonObj["glutenFree"].toBool();
        diateryInfo.lactoseFree = diateryInfoJsonObj["lactoseFree"].toBool();

        QList<QString> instructions;
        QJsonArray instructionsJsonArr = recipeJsonObj["instructions"].toArray();
        for (const QJsonValue &instructionVal: instructionsJsonArr) {
            instructions.push_back(instructionVal.toString());
        }

        if (recipeType == "food") {
            FoodRecipe* recipeObj = new FoodRecipe(title, description, photos, recipeStats, ingredients, nutrition, instructions, diateryInfo, recipeJsonObj["serves"].toInt());
            foodRecipes.push_back(recipeObj);
        } else {
            DrinkRecipe* recipeObj = new DrinkRecipe(title, description, photos, recipeStats, ingredients, nutrition, instructions, diateryInfo, recipeJsonObj["alcoholic"].toBool());
            drinkRecipes.push_back(recipeObj);
        }
    }

    recipesFile.close();

    /*
     * END JSON READING
     */



    /*
     *  BEGIN RECIPE DETAILS PAGE UI SET UP
     */

    rd_grid = new QGridLayout();
    rd_grid->setSpacing(9);
    rd_title = new QLabel();
    rd_description = new QLabel();
    rd_description->setWordWrap(true);
    rd_image = new QLabel();
    rd_stats = new QLabel();
    rd_ingredients = new QLabel();
    rd_nutrition = new QLabel();
    rd_diateryInfo = new QHBoxLayout();
    rd_instructions = new QLabel();

    rd_grid->addWidget(rd_title, 0, 0);
    rd_grid->addWidget(rd_description, 1, 0, 1, 3);
    rd_grid->addWidget(rd_image, 2, 0, 1, 2);
    rd_grid->addLayout(rd_diateryInfo, 3, 0, 1, 3);
    rd_grid->addWidget(rd_stats, 4, 0);
    rd_grid->addWidget(rd_ingredients, 4, 1);
    rd_grid->addWidget(rd_nutrition, 4, 2);
    rd_grid->addWidget(rd_instructions, 5, 0, 1, 3);

    QString rd_sectionStylesheet = "background: #fff;padding: 10px;border-radius:12px;border: 8px solid #e0e0e0;";
    rd_stats->setStyleSheet(rd_sectionStylesheet);
    rd_ingredients->setStyleSheet(rd_sectionStylesheet);
    rd_nutrition->setStyleSheet(rd_sectionStylesheet);
    rd_instructions->setStyleSheet(rd_sectionStylesheet);
    rd_instructions->setWordWrap(true);
    rd_stats->setAlignment(Qt::AlignTop);
    rd_nutrition->setAlignment(Qt::AlignTop);
    rd_ingredients->setAlignment(Qt::AlignTop);

    ui->gridLayout_p2->addLayout(rd_grid, 1, 0, 1, 3);

//    ui->gridLayout_p2->setRowStretch(1, 0);
//    ui->gridLayout_p2->setRowStretch(0, 0);
//    rd_grid->setRowStretch(0, 0);
//    rd_grid->setRowStretch(1, 0);
//    rd_grid->setRowStretch(2, 0);
//    rd_grid->setRowStretch(3, 0);
//    rd_grid->setRowStretch(4, 0);
//    rd_grid->setRowStretch(5, 0);
//    QSpacerItem* spacer = new QSpacerItem(0, 500, QSizePolicy::Maximum, QSizePolicy::Maximum);
//    ui->gridLayout_p2->addItem(spacer, 2, 0);

    /*
     *  END RECIPE DETAILS PAGE UI SET UP
     */



    // now ready to display recipes on screen
    displayCards();

    // working on copy constructor
    //    qDebug() << &drinkRecipes.at(0)->instructions;
    //    DrinkRecipe copy = *drinkRecipes.at(0);
    //    qDebug() << &copy.instructions;


    // ensure page 0 (home) of stacked widget is displayed
    ui->stackedWidget->setCurrentIndex(0);
    setCentralWidget(ui->scrollArea);

}

void MainWindow::displayCards() {

    // clear all cards first and then start fresh
    removeWidgets(ui->gridLayout, 2);

    QList<Recipe*> recipes;
    for (int i = 0; i < foodRecipes.count(); i++) {
        recipes.append(foodRecipes.at(i));
    }
    for (int i = 0; i < drinkRecipes.count(); i++) {
        recipes.append(drinkRecipes.at(i));
    }



    int i = 0;
    for (int j = 0; j < recipes.count(); j++) {

        Recipe* r = recipes.at(j);

        // enforce filters
        if (showFoodOnly && !dynamic_cast<FoodRecipe*>(r)) continue;
        if (showDrinksOnly && !dynamic_cast<DrinkRecipe*>(r)) continue;
        if (showVeganOnly && !(r->dietaryInfo.vegan)) continue;
        if (showVegetarianOnly && !(r->dietaryInfo.vegetarian)) continue;
        if (maxTime < (r->stats.prepTime + r->stats.cookTime)) continue;

        QVBoxLayout* recipeContainer = r->createCard();

        QPushButton* viewBtn = r->viewBtn;
        connect(viewBtn, &QPushButton::clicked, this, [=]() {
            ui->stackedWidget->setCurrentIndex(1);

            rd_title->setText(r->title);
            rd_title->setStyleSheet("font-size: 30px;font-weight:bold;");
            rd_description->setText(r->getDescription());
            QPixmap pm(":/images/" + r->photos.at(0));
            rd_image->setPixmap(pm.scaledToWidth(400));
            rd_stats->setText("<h1>Stats</h1><ul><li>Prep time: " + QString::number(r->stats.prepTime) + "</li>" +
                              "<li>Cook time: " + QString::number(r->stats.cookTime) + "</li>" +
                              "<li>Difficulty level: " + (char) (r->stats.difficulty.difficultyScale + 65) + "</li></ul>"
                              );

            removeWidgets(rd_diateryInfo);
            rd_diateryInfo->addLayout(RecipeUtils::getLabelsComponent(r));

            QString ingredientsHtml;
            ingredientsHtml += "<h1>Ingredients</h1><ul>";
            for (int l = 0; l < r->ingredients.length(); l++) {
                ingredientsHtml += "<li>" + r->ingredients.at(l).quantity + " " + r->ingredients.at(l).title + "</li>";
            }
            ingredientsHtml += "</ul>";
            rd_ingredients->setText(ingredientsHtml);

            QString instructionsHtml;
            instructionsHtml += "<h1>Instructions</h1><ol>";
            for (int l = 0; l < r->instructions.length(); l++) {
                instructionsHtml += "<li>" + r->instructions.at(l) + "</li>";
            }
            instructionsHtml += "</ol>";
            rd_instructions->setText(instructionsHtml);

            QString nutritionHtml = "<h1>Nutrition</h1><ul>";
            nutritionHtml += "<li>" + QString::number(r->nutrition.kcal) + " kcal</li>";
            nutritionHtml += "<li>" + QString::number(r->nutrition.fat) + "g fat</li>";
            nutritionHtml += "<li>" + QString::number(r->nutrition.saturates) + "g saturates</li>";
            nutritionHtml += "<li>" + QString::number(r->nutrition.carbs) + "g carbs</li>";
            nutritionHtml += "<li>" + QString::number(r->nutrition.sugars) + "g sugars</li>";
            nutritionHtml += "<li>" + QString::number(r->nutrition.fibre) + "g fibre</li>";
            nutritionHtml += "<li>" + QString::number(r->nutrition.protein) + "g protein</li>";
            nutritionHtml += "<li>" + QString::number(r->nutrition.salt) + "g salt</li></ul>";
            rd_nutrition->setText(nutritionHtml);

        });

        if (j + 1 < recipes.count() && *r < *recipes.at(j+1)) {
            QLabel* l = new QLabel("Fewer calories (" + QString::number(r->nutrition.kcal) + ") than " + recipes.at(j+1)->title + " (" + QString::number(recipes.at(j+1)->nutrition.kcal) + ")!");
            recipeContainer->insertWidget(recipeContainer->count() - 2, l);
        }
        ui->gridLayout->addLayout(recipeContainer, (i / NUM_CARDS_PER_LINE) + GRID_TOP_OFFSET, i % 4);
        //            ui->gridLayout->setRowMinimumHeight((i / NUM_CARDS_PER_LINE) + GRID_TOP_OFFSET, 600);
        ui->gridLayout->setRowStretch((i / NUM_CARDS_PER_LINE) + GRID_TOP_OFFSET, 0);
        i++;
    }
}

void MainWindow::removeWidgets(QLayout* layout, int startIndex) {
    if (!layout) return;
    // auto so type is determined from thing on RHS
    while (auto elem = layout->takeAt(startIndex)) {
        // memory management
        delete elem->widget();
        // recursive call
        removeWidgets(elem->layout(), 0);
    }
}

void MainWindow::contextMenuEvent(QContextMenuEvent* e) {
    QMenu menu(this);
    menu.addAction(resetAct);
    menu.exec(e->globalPos());
}

MainWindow::~MainWindow()
{
    // memory management
    delete ui;
}


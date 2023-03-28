#include "foodrecipe.h"
#include "utils.h"

#include <QLabel>
#include <QPushButton>
#include <QScreen>
#include <QVBoxLayout>

#include "qscreen.h"
#include <qguiapplication.h>

#include <sstream>
#include <iostream>


FoodRecipe::FoodRecipe(QString title, QString description, QList<QString> photos, RecipeStats stats, QList<Ingredient> ingredients,
                       Nutrition nutrition, bool vegan, bool vegetarian, QList<QString> instructions, DietaryInfo dietaryInfo, int servings)
    : Recipe(title, description, photos, stats, ingredients, nutrition, vegan, vegetarian, instructions, dietaryInfo), servings(servings)
{
    qDebug() << photos.size();
}

int FoodRecipe::getServings() {
    return servings;
}

QVBoxLayout* FoodRecipe::createCard() {


    QVBoxLayout* recipeContainer = new QVBoxLayout();
    recipeContainer->setSpacing(10);

    // image
    QLabel* image = new QLabel();
    QPixmap pix(":/images/" + photos.first());
    recipeContainer->addWidget(image);

    QScreen *screen = QGuiApplication::primaryScreen();
    int width = screen->geometry().width();

    image->setScaledContents(true);
    //image->setPixmap(pix.scaled( image->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
//    image->setPixmap(pix.scaled(300, 200));
    image->setPixmap(pix.scaledToWidth((width - 200) / 4));

    // title
    QLabel* titleLabel = Recipe::getCardTitleComponent();
    recipeContainer->addWidget(titleLabel);

    QHBoxLayout* labels = getLabelsComponent(this);
    recipeContainer->addLayout(labels);


    // stats grid
    QGridLayout* statsGrid = Recipe::getCardStatsGridComponent();

    // serving count
    QLabel* stats_servingCountIconLabel = new QLabel;
    QPixmap stats_servingCountIcon(":/images/icon_restaurant.svg");
    QLabel* stats_servingCountValueLabel = new QLabel(QString::number(servings));
    stats_servingCountValueLabel->setStyleSheet("background: #fff;");
    stats_servingCountIconLabel->setPixmap(stats_servingCountIcon);
    statsGrid->addWidget(stats_servingCountIconLabel, 1, 0);
    statsGrid->addWidget(stats_servingCountValueLabel, 1, 1, 1, 4);

    recipeContainer->addLayout(statsGrid);


    // serving count
    /*QLabel* stats_servingCountIconLabel = new QLabel;
    QPixmap stats_servingCountIcon(":/images/icon_restaurant.svg");
    QLabel* stats_servingCountValueLabel = new QLabel(QString::number(servings));
    stats_servingCountValueLabel->setStyleSheet("background: #fff;");
    stats_servingCountIconLabel->setPixmap(stats_servingCountIcon);
    statsGrid->addWidget(stats_servingCountIconLabel, 1, 0);
    statsGrid->addWidget(stats_servingCountValueLabel, 1, 1, 1, 4);*/


    // description
    QLabel* descriptionLabel = Recipe::getCardDescriptionComponent();
    recipeContainer->addWidget(descriptionLabel);


    // view button
    QPushButton* viewBtn = Recipe::getCardButtonComponent();
    recipeContainer->addWidget(viewBtn);

    return recipeContainer;
}

std::ostringstream &operator<<(std::ostringstream &o, const FoodRecipe &r) {
    qDebug() << "<< operator called";
    o << "test ostream";
    return o;
}

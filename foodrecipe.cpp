#include "foodrecipe.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include <sstream>
#include <iostream>


FoodRecipe::FoodRecipe(QString title, QString description, QList<QString> photos, RecipeStats stats, QList<Ingredient> ingredients,
                       Nutrition nutrition, bool vegan, bool vegetarian, QList<QString> instructions, int servings)
    : Recipe(title, description, photos, stats, ingredients, nutrition, vegan, vegetarian, instructions), servings(servings)
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

    //image->setScaledContents(true);
    //image->setPixmap(pix.scaled( image->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    image->setPixmap(pix.scaled(300, 200));

    // title
    QLabel* titleLabel = Recipe::getCardTitleComponent();
    recipeContainer->addWidget(titleLabel);


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

    recipeContainer->addLayout(statsGrid);

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

#include "foodrecipe.h"
#include "utils.h"

#include <QLabel>
#include <QPushButton>
#include <QScreen>
#include <QVBoxLayout>
#include <Qguiapplication>

#include <sstream>
#include <iostream>


FoodRecipe::FoodRecipe(QString title, QString description, QList<QString> photos, RecipeStats stats, QList<Ingredient> ingredients,
                       Nutrition nutrition, QList<QString> instructions, DietaryInfo dietaryInfo, int servings)
    : Recipe(title, description, photos, stats, ingredients, nutrition, instructions, dietaryInfo), servings(servings)
{

}

int FoodRecipe::getServings() {
    return servings;
}

QVBoxLayout* FoodRecipe::createCard() {

    QVBoxLayout* recipeContainer = new QVBoxLayout();
    recipeContainer->setSpacing(10);

    // image
    QLabel* image = Recipe::getCardPhotosComponent();
    recipeContainer->addWidget(image);

    // title
    QLabel* titleLabel = Recipe::getCardTitleComponent();
    recipeContainer->addWidget(titleLabel);

    QHBoxLayout* labels = RecipeUtils::getLabelsComponent(this);
    recipeContainer->addLayout(labels);

    // stats grid
    QGridLayout* statsGrid = Recipe::getCardStatsGridComponent();

    // serving count
    QLabel* stats_servingCountIconLabel = new QLabel;
    QPixmap stats_servingCountIcon(":/images/icon_restaurant.svg");
    QLabel* stats_servingCountValueLabel = new QLabel("Serves " + QString::number(servings));
    stats_servingCountValueLabel->setStyleSheet("background: #fff;padding:5px;");
    stats_servingCountIconLabel->setPixmap(stats_servingCountIcon);
    statsGrid->addWidget(stats_servingCountIconLabel, 1, 0);
    statsGrid->addWidget(stats_servingCountValueLabel, 1, 1, 1, 4);

    recipeContainer->addLayout(statsGrid);

    // description
    QLabel* descriptionLabel = Recipe::getCardDescriptionComponent();
    recipeContainer->addWidget(descriptionLabel);

    // view button
    viewBtn = Recipe::getCardButtonComponent();
    recipeContainer->addWidget(viewBtn);

    return recipeContainer;
}

std::string FoodRecipe::to_short_string() {
    return title.toStdString() + " - " + description.toStdString();
}

std::string FoodRecipe::to_long_string() {
    return "Food recipe:\n" + Recipe::to_long_string();
}

std::ostringstream &operator<<(std::ostringstream &o, FoodRecipe &r) {
    o << r.to_long_string();
    return o;
}

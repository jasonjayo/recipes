#include "ingredient.h"
#include "recipe.h"

#include <QString>
#include <QList>
#include <iostream>
#include <QLabel>
#include <QPushButton>

#include <sstream>
#include <iostream>

Recipe::Recipe(QString title, QString description, QList<QString> photos, RecipeStats stats, QList<Ingredient> ingredients, Nutrition nutrition, bool vegan, bool vegetarian, QList<QString> instructions, DietaryInfo dietaryInfo)
    : title(title), description(description), photos(photos), dietaryInfo(dietaryInfo), stats(stats), ingredients(ingredients), nutrition(nutrition), vegan(vegan), vegetarian(vegetarian), instructions(instructions)
{
}

Recipe::Recipe(const Recipe& otherRecipe) {
    qDebug() << "copy";
    title = QString(otherRecipe.title);
    description = QString(otherRecipe.description);
    // deep copy performed for QList automatically by Qt when this copy modified, so otherRecipe doesn't get modified   as well
    photos = otherRecipe.photos;
    stats.cookTime = otherRecipe.stats.cookTime;
    stats.prepTime = otherRecipe.stats.prepTime;
    stats.difficulty = otherRecipe.stats.difficulty;
    ingredients = otherRecipe.ingredients;
    nutrition.carbs = otherRecipe.nutrition.carbs;
    nutrition.fat = otherRecipe.nutrition.fat;
    nutrition.fibre = otherRecipe.nutrition.fibre;
    nutrition.kcal = otherRecipe.nutrition.kcal;
    nutrition.protein = otherRecipe.nutrition.protein;
    nutrition.salt = otherRecipe.nutrition.salt;
    nutrition.saturates = otherRecipe.nutrition.saturates;
    nutrition.sugars = otherRecipe.nutrition.sugars;
    vegan = otherRecipe.vegan;
    vegetarian = otherRecipe.vegetarian;
    instructions = otherRecipe.instructions;
}

QLabel* Recipe::getCardTitleComponent() {
    QLabel* titleLabel = new QLabel(title);
    titleLabel->setStyleSheet("QLabel {font-size: 26px; background: #fff; font-weight:bold; padding: 6px;}");
    titleLabel->setWordWrap(true);

    return titleLabel;
}

QGridLayout* Recipe::getCardStatsGridComponent() {
    // stats grid
    QGridLayout* statsGrid = new QGridLayout;

    // prep time
    QLabel* stats_timeIconLabel = new QLabel;
    QLabel* stats_timeValue = new QLabel(QString("Prep in " + QString::number(stats.prepTime) + " minutes"));
    QPixmap stats_timeIcon(":/images/icon_clock.svg");
    stats_timeValue->setStyleSheet("background: #fff;");
    stats_timeIconLabel->setPixmap(stats_timeIcon);
    statsGrid->addWidget(stats_timeIconLabel, 0, 0);
    statsGrid->addWidget(stats_timeValue, 0, 1, 1, 4);

    return statsGrid;
}

QLabel* Recipe::getCardDescriptionComponent() {
    QLabel* descriptionLabel = new QLabel(description);
    descriptionLabel->setWordWrap(true);
    descriptionLabel->setStyleSheet("background:#fff; padding: 6px;border: 4px solid #cfcfcf");
    return descriptionLabel;
}

QPushButton* Recipe::getCardButtonComponent() {
    QPushButton* viewBtn = new QPushButton;
    viewBtn->setText("View recipe");
    viewBtn->setStyleSheet("QPushButton { background: #cfcfcf; border: none; padding: 8px 0; font-weight:bold; text-transform:uppercase; font-size: 12px; }QPushButton:hover {background: #fff}");
    return viewBtn;
}

//QHBoxLayout* Recipe::getLabelsComponent() {
//    QHBoxLayout* box = new QHBoxLayout();
//    if (dietaryInfo.vegetarian) {
//        QLabel* vegetarian = new QLabel("Vegetarian");
//        vegetarian->setStyleSheet("background: #4caf50; padding: 2px 5px;");
//        box->addWidget(vegetarian);
//    }
//    if (dietaryInfo.vegan) {
//        QLabel* vegetarian = new QLabel("Vegan");
//        vegetarian->setStyleSheet("background: #8bc34a; padding: 2px 5px;");
//        box->addWidget(vegetarian);
//    }
//    if (dietaryInfo.lactoseFree) {
//        QLabel* vegetarian = new QLabel("Lactose Free");
//        vegetarian->setStyleSheet("background: #cddc39; padding: 2px 5px;");
//        box->addWidget(vegetarian);
//    }
//    if (dietaryInfo.glutenFree) {
//        QLabel* vegetarian = new QLabel("Gluten Free");
//        vegetarian->setStyleSheet("background: #009688; padding: 2px 5px;");
//        box->addWidget(vegetarian);
//    }
//    return box;
//}

double Recipe::operator+(Recipe const& r) {
    return nutrition.kcal + r.nutrition.kcal;
}

std::ostream &operator<<(std::ostream&o, const Recipe &r) {
    qDebug() << "<< operator called";
    o << "test ostream";
    return o;
}

bool operator< (const Recipe &r1, const Recipe &r2) {
    qDebug() << "< called - checking " + QString::number(r1.nutrition.kcal) + " and " + QString::number(r2.nutrition.kcal);
    return r1.nutrition.kcal < r2.nutrition.kcal;
}
bool operator> (const Recipe &r1, const Recipe &r2) {
    qDebug() << "> called";
    return r1.nutrition.kcal > r2.nutrition.kcal;
}

#ifndef RECIPE_H
#define RECIPE_H

#include "ingredient.h"

#include <QString>
#include <QList>
#include <iostream>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

struct RecipeStats {
    int prepTime;
    int cookTime;
    int difficulty;
};

struct Nutrition {
    double kcal;
    double fat;
    double saturates;
    double carbs;
    double sugars;
    double fibre;
    double protein;
    double salt;
};

class Recipe
{
public:
    Recipe(QString title, QString description, QList<QString> photos, RecipeStats stats, QList<Ingredient> ingredients,
           Nutrition nutrition, bool vegan, bool vegetarian, QList<QString> instructions);


    virtual QVBoxLayout* createCard() = 0;
//    virtual QString getDescription() = 0;

    QString title;
    QString description;
    QList<QString> photos;



    RecipeStats stats;

    QList<Ingredient> ingredients;



    Nutrition nutrition;

    bool vegan;
    bool vegetarian;
    QList<QString> instructions;

    Recipe(const Recipe &otherRecipe);
protected:
    virtual QLabel *getCardTitleComponent();
    virtual QGridLayout *getCardStatsGridComponent();
    virtual QLabel *getCardDescriptionComponent();
    virtual QPushButton *getCardButtonComponent();
};

#endif // RECIPE_H

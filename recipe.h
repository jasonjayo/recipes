#ifndef RECIPE_H
#define RECIPE_H

#include "ingredient.h"

#include <QString>
#include <QList>
#include <iostream>

struct RecipeStats {
    int prepTime;
    int cookTime;
    int difficulty;
    int serves;
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
    Recipe(QString title, QString description, RecipeStats stats, QList<Ingredient> ingredients,
           Nutrition nutrition, bool vegan, bool vegetarian, QList<QString> instructions);

    //Recipe(QString title);

    QString title;
    QString description;



    RecipeStats stats;

    QList<Ingredient> ingredients;



    Nutrition nutrition;

    bool vegan;
    bool vegetarian;
    QList<QString> instructions;

};

#endif // RECIPE_H

#ifndef RECIPE_H
#define RECIPE_H

#include <QString>
#include <QList>

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
    Recipe(QString title, QString description, RecipeStats stats, QList<QString> ingredients,
           Nutrition nutrition, bool vegan, bool vegetarian, QList<QString> instructions);

    QString title;
    QString description;



    RecipeStats stats;

    QVector<QString> ingredients;



    Nutrition nutrition;

    bool vegan;
    bool vegetarian;
    QVector<QString> instructions;
};

#endif // RECIPE_H

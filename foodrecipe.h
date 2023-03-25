#ifndef FOODRECIPE_H
#define FOODRECIPE_H

#include "recipe.h"
#include <QVBoxLayout>

class FoodRecipe : public Recipe
{
public:
    FoodRecipe(QString title, QString description, QList<QString> photos, RecipeStats stats, QList<Ingredient> ingredients,
               Nutrition nutrition, bool vegan, bool vegetarian, QList<QString> instructions, int servings);

    QVBoxLayout* createCard();
    int servings;
    int getServings();
};

#endif // FOODRECIPE_H

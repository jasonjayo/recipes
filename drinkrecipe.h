#ifndef DRINKRECIPE_H
#define DRINKRECIPE_H

#include "recipe.h"

class DrinkRecipe : public Recipe
{
public:
    DrinkRecipe(QString title, QString description, QList<QString> photos, RecipeStats stats, QList<Ingredient> ingredients,
                Nutrition nutrition, bool vegan, bool vegetarian, QList<QString> instructions, bool alcoholic);

    bool alcoholic;
    QVBoxLayout* createCard();
};

#endif // DRINKRECIPE_H

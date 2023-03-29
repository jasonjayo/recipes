#ifndef DRINKRECIPE_H
#define DRINKRECIPE_H

#include "recipe.h"

class DrinkRecipe : public Recipe
{
public:
    DrinkRecipe(QString title, QString description, QList<QString> photos, RecipeStats stats, QList<Ingredient> ingredients,
                Nutrition nutrition, bool vegan, bool vegetarian, QList<QString> instructions, DietaryInfo dietaryInfo, bool alcoholic);

    bool alcoholic;
    QVBoxLayout* createCard();

    std::string to_short_string();
    std::string to_long_string();

    QPushButton* viewBtn;
};

#endif // DRINKRECIPE_H

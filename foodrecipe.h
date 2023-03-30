#ifndef FOODRECIPE_H
#define FOODRECIPE_H

#include "recipe.h"
#include "stringable.h"
#include <QVBoxLayout>

class FoodRecipe : public Recipe, protected Stringable
{
public:
    FoodRecipe(QString title, QString description, QList<QString> photos, RecipeStats stats, QList<Ingredient> ingredients,
               Nutrition nutrition, bool vegan, bool vegetarian, QList<QString> instructions, DietaryInfo dietaryInfo, int servings);

    QVBoxLayout* createCard();
    int servings;
    int getServings();
    friend std::ostringstream &operator<<(std::ostringstream &o, const FoodRecipe &r);

    std::string to_short_string();
    std::string to_long_string();
};

#endif // FOODRECIPE_H

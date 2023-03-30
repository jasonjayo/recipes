#ifndef FOODRECIPE_H
#define FOODRECIPE_H

#include "recipe.h"
#include "stringable.h"
#include <QVBoxLayout>

// multiple inheritance
class FoodRecipe : public Recipe, protected Stringable
{
public:
    // initialiser list
    FoodRecipe(QString title, QString description, QList<QString> photos, RecipeStats stats, QList<Ingredient> ingredients, Nutrition nutrition, QList<QString> instructions, DietaryInfo dietaryInfo, int servings);

    int servings;

    QVBoxLayout* createCard();
    int getServings();
    std::string to_short_string();
    std::string to_long_string();

    friend std::ostringstream &operator<<(std::ostringstream &o, const FoodRecipe &r);
};

#endif // FOODRECIPE_H

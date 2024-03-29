#ifndef DRINKRECIPE_H
#define DRINKRECIPE_H

#include "recipe.h"
#include "stringable.h"

// multiple inheritance
class DrinkRecipe : public Recipe, protected Stringable
{
public:
    DrinkRecipe(QString title, QString description, QList<QString> photos, RecipeStats stats, QList<Ingredient> ingredients, Nutrition nutrition, QList<QString> instructions, DietaryInfo dietaryInfo, bool alcoholic);

    bool alcoholic;

    QVBoxLayout* createCard();
    std::string to_short_string();
    std::string to_long_string();
    QString getDescription();
    QLabel* getCardDescriptionComponent();
};

#endif // DRINKRECIPE_H

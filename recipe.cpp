#include "recipe.h"

#include <QString>
#include <QList>

Recipe::Recipe(QString title, QString description, RecipeStats stats, QList<QString> ingredients,
               Nutrition nutrition, bool vegan, bool vegetarian, QList<QString> instructions)
    : title(title), description(description), stats(stats), ingredients(ingredients), nutrition(nutrition),
    vegan(vegan), vegetarian(vegetarian), instructions(instructions)
{
}

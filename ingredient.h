#ifndef INGREDIENT_H
#define INGREDIENT_H

#include <QString>



class Ingredient
{
public:
    Ingredient(QString title, QString quantity, bool vegan = false, bool vegetarian = false);
    QString quantity;
    QString title;
    bool vegan;
    bool vegetarian;
};

#endif // INGREDIENT_H

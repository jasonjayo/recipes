#ifndef RECIPE_H
#define RECIPE_H

#include "ingredient.h"

#include <QString>
#include <QList>
#include <iostream>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

struct RecipeStats {
    int prepTime;
    int cookTime;
    int difficulty;
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

// bit structures
struct DietaryInfo {
    unsigned vegan: 1;
    unsigned vegetarian: 1;
    unsigned glutenFree: 1;
    unsigned lactoseFree: 1;
};

// abstract class
// virtual, pure virtual functions
class Recipe
{
public:
    Recipe(QString title, QString description, QList<QString> photos, RecipeStats stats, QList<Ingredient> ingredients,
           Nutrition nutrition, QList<QString> instructions, DietaryInfo dietaryInfo);



    QString title;
    QString description;
    QList<QString> photos;
    DietaryInfo dietaryInfo;
    RecipeStats stats;
    QList<Ingredient> ingredients;
    Nutrition nutrition;
    bool vegan;
    bool vegetarian;
    QList<QString> instructions;
    QPushButton* viewBtn;

    Recipe(const Recipe &otherRecipe);
    virtual QVBoxLayout* createCard() = 0;
    friend std::ostream &operator<<(std::ostream &o, Recipe &r);
    friend bool operator< (const Recipe &r1, const Recipe &r2);
    friend bool operator> (const Recipe &r1, const Recipe &r2);
    double operator +(const Recipe &r);
    std::string to_long_string();
protected:
    virtual QLabel *getCardTitleComponent();
    virtual QGridLayout *getCardStatsGridComponent();
    virtual QLabel *getCardDescriptionComponent();
    virtual QPushButton *getCardButtonComponent();
};

#endif // RECIPE_H

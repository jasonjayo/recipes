#include "drinkrecipe.h"
#include "utils.h"

#include <QLabel>
#include <QPushButton>
#include <Qguiapplication>
#include <QHBoxLayout>

DrinkRecipe::DrinkRecipe(QString title, QString description, QList<QString> photos, RecipeStats stats, QList<Ingredient> ingredients, Nutrition nutrition, QList<QString> instructions, DietaryInfo dietaryInfo, bool alcoholic)
    : Recipe(title, description, photos, stats, ingredients, nutrition, instructions, dietaryInfo), alcoholic(alcoholic)
{

}

QVBoxLayout* DrinkRecipe::createCard() {

    QVBoxLayout* recipeContainer = new QVBoxLayout();
    recipeContainer->setSpacing(10);

    // image
    QLabel* image = Recipe::getCardPhotosComponent();
    recipeContainer->addWidget(image);

    // title
    QLabel* titleLabel = getCardTitleComponent();
    recipeContainer->addWidget(titleLabel);

    QHBoxLayout* labels = RecipeUtils::getLabelsComponent(this);
    recipeContainer->addLayout(labels);

    // stats grid
    QGridLayout* statsGrid = Recipe::getCardStatsGridComponent();
    recipeContainer->addLayout(statsGrid);

    // description
    QLabel* descriptionLabel = getCardDescriptionComponent();
    recipeContainer->addWidget(descriptionLabel);

    // view button
    viewBtn = Recipe::getCardButtonComponent();
    recipeContainer->addWidget(viewBtn);

    return recipeContainer;
}

QLabel* DrinkRecipe::getCardDescriptionComponent() {
    QLabel* descriptionLabel = Recipe::getCardDescriptionComponent();
    if (alcoholic) {
        descriptionLabel->setText(descriptionLabel->text() + "<br><br><i>Contains alcohol</i>");
        descriptionLabel->setTextFormat(Qt::RichText);
    }
    return descriptionLabel;
}

QString DrinkRecipe::getDescription() {
    if (alcoholic) return description + " " + "<i>Contains alcohol.</i>";
    return description;
}

std::string DrinkRecipe::to_short_string() {
    return title.toStdString() + " - " + description.toStdString();
}

std::string DrinkRecipe::to_long_string() {
    return "Drink recipe:\n" + Recipe::to_long_string();
}



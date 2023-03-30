#include "drinkrecipe.h"
#include "qscreen.h"
#include "utils.h"

#include <QLabel>
#include <QPushButton>
#include <qguiapplication.h>
#include <QHBoxLayout>

DrinkRecipe::DrinkRecipe(QString title, QString description, QList<QString> photos, RecipeStats stats, QList<Ingredient> ingredients, Nutrition nutrition, QList<QString> instructions, DietaryInfo dietaryInfo, bool alcoholic)
    : Recipe(title, description, photos, stats, ingredients, nutrition, instructions, dietaryInfo), alcoholic(alcoholic)
{

}

QVBoxLayout* DrinkRecipe::createCard() {

    QVBoxLayout* recipeContainer = new QVBoxLayout();
    recipeContainer->setSpacing(10);

    // image
    QLabel* image = new QLabel();
    QPixmap pix(":/images/" + photos.first());
    recipeContainer->addWidget(image);
    QScreen *screen = QGuiApplication::primaryScreen();
    int width = screen->geometry().width();
    image->setScaledContents(true);
    image->setPixmap(pix.scaledToWidth((width - 200) / 4));
    image->setToolTip(QString::fromStdString(to_long_string()));

    // title
    QLabel* titleLabel = getCardTitleComponent();
    recipeContainer->addWidget(titleLabel);

    QHBoxLayout* labels = RecipeUtils::getLabelsComponent(this);
    recipeContainer->addLayout(labels);

    // stats grid
    QGridLayout* statsGrid = Recipe::getCardStatsGridComponent();
    recipeContainer->addLayout(statsGrid);

    // description
    QLabel* descriptionLabel = Recipe::getCardDescriptionComponent();
    recipeContainer->addWidget(descriptionLabel);

    // view button
    viewBtn = Recipe::getCardButtonComponent();
    recipeContainer->addWidget(viewBtn);

    return recipeContainer;
}

std::string DrinkRecipe::to_short_string() {
    return title.toStdString() + " - " + description.toStdString();
}

std::string DrinkRecipe::to_long_string() {
    return "Drink recipe:\n" + Recipe::to_long_string();
}



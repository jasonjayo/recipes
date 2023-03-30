#include "ingredient.h"
#include "recipe.h"

#include <QString>
#include <QList>
#include <QLabel>
#include <QPushButton>
#include <QObject>
#include <QGuiApplication>
#include "Qscreen"

Recipe::Recipe(QString title, QString description, QList<QString> photos, RecipeStats stats, QList<Ingredient> ingredients, Nutrition nutrition, QList<QString> instructions, DietaryInfo dietaryInfo)
    : title(title), description(description), photos(photos), dietaryInfo(dietaryInfo), stats(stats), ingredients(ingredients), nutrition(nutrition), instructions(instructions)
{
}

// deep copy constructor
Recipe::Recipe(const Recipe& otherRecipe) {
    title = QString(otherRecipe.title);
    description = QString(otherRecipe.description);
    // deep copy performed for QList automatically by Qt when this copy modified, so otherRecipe doesn't get modified   as well
    photos = otherRecipe.photos;
    stats.cookTime = otherRecipe.stats.cookTime;
    stats.prepTime = otherRecipe.stats.prepTime;
    stats.difficulty.numericalVal = otherRecipe.stats.difficulty.numericalVal;
    ingredients = otherRecipe.ingredients;
    nutrition.carbs = otherRecipe.nutrition.carbs;
    nutrition.fat = otherRecipe.nutrition.fat;
    nutrition.fibre = otherRecipe.nutrition.fibre;
    nutrition.kcal = otherRecipe.nutrition.kcal;
    nutrition.protein = otherRecipe.nutrition.protein;
    nutrition.salt = otherRecipe.nutrition.salt;
    nutrition.saturates = otherRecipe.nutrition.saturates;
    nutrition.sugars = otherRecipe.nutrition.sugars;
    instructions = otherRecipe.instructions;
}

QLabel* Recipe::getCardTitleComponent() {
    QLabel* titleLabel = new QLabel(title);
    titleLabel->setStyleSheet("QLabel {font-size: 26px; background: #fff; font-weight:bold; padding: 6px;}");
    titleLabel->setWordWrap(true);

    return titleLabel;
}

QGridLayout* Recipe::getCardStatsGridComponent() {
    // stats grid
    QGridLayout* statsGrid = new QGridLayout;

    // prep time
    QLabel* stats_timeIconLabel = new QLabel;
    QLabel* stats_timeValue = new QLabel(QString("Prep in " + QString::number(stats.prepTime) + " minutes\n" +
                              "Cook in " + QString::number(stats.cookTime) + " minutes"));
    QPixmap stats_timeIcon(":/images/icon_clock.svg");
    stats_timeValue->setStyleSheet("background: #fff; padding: 5px;");
    stats_timeIconLabel->setPixmap(stats_timeIcon);
    statsGrid->addWidget(stats_timeIconLabel, 0, 0);
    statsGrid->addWidget(stats_timeValue, 0, 1, 1, 4);

    return statsGrid;
}

QLabel* Recipe::getCardDescriptionComponent() {
    QLabel* descriptionLabel = new QLabel(description);
    descriptionLabel->setWordWrap(true);
    descriptionLabel->setStyleSheet("background:#fff; padding: 6px;border: 4px solid #e0e0e0");
    return descriptionLabel;
}

QPushButton* Recipe::getCardButtonComponent() {
    QPushButton* viewBtn = new QPushButton;
    viewBtn->setText("View recipe");
    viewBtn->setStyleSheet("QPushButton { background: #e0e0e0; border: none; padding: 12px 0; font-weight:bold; text-transform:uppercase; font-size: 12px; }QPushButton:hover {background: #fff}");

    return viewBtn;
}

QLabel* Recipe::getCardPhotosComponent() {
    QLabel* image = new QLabel();
    QPixmap pix(":/images/" + photos.first());
    QScreen *screen = QGuiApplication::primaryScreen();
    int width = screen->geometry().width();
    image->setScaledContents(true);
    image->setPixmap(pix.scaledToWidth((width - 200) / 4));
    image->setToolTip(QString::fromStdString(to_long_string()));
    return image;
}

QString Recipe::getDescription() {
    return description;
}

// destructor & memory management
Recipe::~Recipe() {
    delete viewBtn;
}

std::string Recipe::to_long_string() {
    std::string s;
    s += title.toStdString() + " - " + description.toStdString() + "\n\n";
    if (dietaryInfo.glutenFree) {
        s += "Gluten free\n";
    }
    if (dietaryInfo.lactoseFree) {
        s += "Lactose free\n";
    }
    if (dietaryInfo.vegan) {
        s += "Vegan\n";
    }
    if (dietaryInfo.vegetarian) {
        s += "Vegetarian\n";
    }
    s += "\n";
    s += QString::number(nutrition.kcal).toStdString() + " kcal";
    return s;
}

// overloaded operator
std::ostream &operator<<(std::ostream&o, Recipe &r) {
    o << r.to_long_string();
    return o;
}

// overloaded operator
bool operator< (const Recipe &r1, const Recipe &r2) {
    return r1.nutrition.kcal < r2.nutrition.kcal;
}

// overloaded operator
bool operator> (const Recipe &r1, const Recipe &r2) {
    return r1.nutrition.kcal > r2.nutrition.kcal;
}

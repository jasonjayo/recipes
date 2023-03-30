#ifndef UTILS_H
#define UTILS_H

#include <QHBoxLayout>
#include <QLabel>

// namespace
namespace RecipeUtils {

// template function
template <class T> QHBoxLayout* getLabelsComponent(T a) {
    QHBoxLayout* box = new QHBoxLayout();
    if (a->dietaryInfo.vegetarian) {
        QLabel* vegetarian = new QLabel("Vegetarian");
        vegetarian->setStyleSheet("background: #4caf50; padding: 2px 5px; border-radius: 6px;");
        vegetarian->setAlignment(Qt::AlignCenter);
        box->addWidget(vegetarian);
    }
    if (a->dietaryInfo.vegan) {
        QLabel* vegan = new QLabel("Vegan");
        vegan->setStyleSheet("background: #8bc34a; padding: 2px 5px; border-radius: 6px;");
        vegan->setAlignment(Qt::AlignCenter);
        box->addWidget(vegan);
    }
    if (a->dietaryInfo.lactoseFree) {
        QLabel* lactoseFree = new QLabel("Lactose Free");
        lactoseFree->setStyleSheet("background: #cddc39; padding: 2px 5px; border-radius: 6px;");
        lactoseFree->setAlignment(Qt::AlignCenter);
        box->addWidget(lactoseFree);
    }
    if (a->dietaryInfo.glutenFree) {
        QLabel* glutenFree = new QLabel("Gluten Free");
        glutenFree->setStyleSheet("background: #009688; padding: 2px 5px; border-radius: 6px;");
        glutenFree->setAlignment(Qt::AlignCenter);
        box->addWidget(glutenFree);
    }
    return box;
}

}

#endif // UTILS_H

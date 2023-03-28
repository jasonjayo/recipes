#ifndef UTILS_H
#define UTILS_H

#include <QHBoxLayout>
#include <QLabel>

template <class T> QHBoxLayout* getLabelsComponent(T a) {
    QHBoxLayout* box = new QHBoxLayout();
    if (a->dietaryInfo.vegetarian) {
        QLabel* vegetarian = new QLabel("Vegetarian");
        vegetarian->setStyleSheet("background: #4caf50; padding: 2px 5px;");
        box->addWidget(vegetarian);
    }
    if (a->dietaryInfo.vegan) {
        QLabel* vegan = new QLabel("Vegan");
        vegan->setStyleSheet("background: #8bc34a; padding: 2px 5px;");
        box->addWidget(vegan);
    }
    if (a->dietaryInfo.lactoseFree) {
        QLabel* lactoseFree = new QLabel("Lactose Free");
        lactoseFree->setStyleSheet("background: #cddc39; padding: 2px 5px;");
        box->addWidget(lactoseFree);
    }
    if (a->dietaryInfo.glutenFree) {
        QLabel* glutenFree = new QLabel("Gluten Free");
        glutenFree->setStyleSheet("background: #009688; padding: 2px 5px;");
        box->addWidget(glutenFree);
    }
    return box;
}

void test();

#endif // UTILS_H

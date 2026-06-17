#ifndef BUBBLESORT_H
#define BUBBLESORT_H

#include "SortingStrategy.h"

class BubbleSort : public SortingStrategy {
public:
    QString name() const override { return "Пузырёк"; }
    QVector<Step> execute(const QVector<int>& array) override;
};

#endif

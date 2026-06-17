#ifndef QUICKSORT_H
#define QUICKSORT_H

#include "SortingStrategy.h"

class QuickSort : public SortingStrategy {
public:
    QString name() const override { return "Быстрая"; }
    QVector<Step> execute(const QVector<int>& array) override;
private:
    void quickSort(QVector<int>& array, int low, int high, QVector<Step>& steps);
    int partition(QVector<int>& array, int low, int high, QVector<Step>& steps);
};

#endif

#ifndef MERGESORT_H
#define MERGESORT_H

#include "SortingStrategy.h"

class MergeSort : public SortingStrategy {
public:
    QString name() const override { return "Слияние"; }
    QVector<Step> execute(const QVector<int>& array) override;

private:
    void mergeSort(QVector<int>& array, int left, int right, QVector<Step>& steps);
    void merge(QVector<int>& array, int left, int mid, int right, QVector<Step>& steps);
};

#endif

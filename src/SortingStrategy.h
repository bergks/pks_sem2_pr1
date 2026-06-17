#ifndef SORTINGSTRATEGY_H
#define SORTINGSTRATEGY_H

#include <QVector>
#include <QString>

enum class StepType {
    Compare,
    Swap,
    PivotSelect,
    Merge,
    Write,
    Finished
};

struct Step {
    QVector<int> arrayState;
    QVector<int> activeIndices = {};
    int pivotIndex = -1;
    StepType type = StepType::Compare;
    QString description = "";
};

class SortingStrategy {
public:
    virtual ~SortingStrategy() = default;
    virtual QString name() const = 0;
    virtual QVector<Step> execute(const QVector<int>& array) = 0;
};

#endif

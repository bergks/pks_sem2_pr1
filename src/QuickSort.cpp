#include "QuickSort.h"
#include <algorithm>

QVector<Step> QuickSort::execute(const QVector<int>& input)
{
    QVector<Step> steps;
    QVector<int> array = input;

    if (!array.isEmpty()) {
        quickSort(array, 0, array.size() - 1, steps);
    }

    Step finishedStep;
    finishedStep.arrayState = array;
    finishedStep.type = StepType::Finished;
    finishedStep.description = "Сортировка завершена";

    steps.append(finishedStep);

    return steps;
}

void QuickSort::quickSort(QVector<int>& array,
                          int low,
                          int high,
                          QVector<Step>& steps)
{
    if (low < high) {

        int pivotIndex = partition(array, low, high, steps);

        quickSort(array, low, pivotIndex - 1, steps);
        quickSort(array, pivotIndex + 1, high, steps);
    }
}

int QuickSort::partition(QVector<int>& array,
                         int low,
                         int high,
                         QVector<Step>& steps)
{
    int pivot = array[high];
    int i = low - 1;

    Step pivotStep;
    pivotStep.arrayState = array;
    pivotStep.pivotIndex = high;
    pivotStep.type = StepType::PivotSelect;
    pivotStep.description =
        QString("Выбираем опорный элемент: %1")
            .arg(pivot);

    steps.append(pivotStep);

    for (int j = low; j < high; ++j) {

        Step compareStep;
        compareStep.arrayState = array;
        compareStep.activeIndices = {j, high};
        compareStep.pivotIndex = high;
        compareStep.type = StepType::Compare;
        compareStep.description =
            QString("Сравниваем %1 с опорным %2")
                .arg(array[j])
                .arg(pivot);

        steps.append(compareStep);

        if (array[j] < pivot) {
            ++i;
            if (i != j) {

                int leftValue = array[i];
                int rightValue = array[j];

                std::swap(array[i], array[j]);

                Step swapStep;
                swapStep.arrayState = array;
                swapStep.activeIndices = {i, j};
                swapStep.pivotIndex = high;
                swapStep.type = StepType::Swap;
                swapStep.description =
                    QString("Меняем %1 и %2 местами")
                        .arg(leftValue)
                        .arg(rightValue);

                steps.append(swapStep);
            }
        }
    }

    std::swap(array[i + 1], array[high]);

    Step finalPivotStep;
    finalPivotStep.arrayState = array;
    finalPivotStep.activeIndices = {i + 1, high};
    finalPivotStep.pivotIndex = i + 1;
    finalPivotStep.type = StepType::Swap;
    finalPivotStep.description =
        QString("Перемещаем опорный элемент %1 на правильную позицию")
            .arg(pivot);

    steps.append(finalPivotStep);

    return i + 1;
}

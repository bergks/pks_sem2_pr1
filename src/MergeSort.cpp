#include "MergeSort.h"

QVector<Step> MergeSort::execute(const QVector<int>& input)
{
    QVector<Step> steps;
    QVector<int> array = input;

    if (!array.isEmpty()) {
        mergeSort(array, 0, array.size() - 1, steps);
    }

    Step finishedStep;
    finishedStep.arrayState = array;
    finishedStep.type = StepType::Finished;
    finishedStep.description = "Сортировка завершена";

    steps.append(finishedStep);

    return steps;
}

void MergeSort::mergeSort(QVector<int>& array,
                          int left,
                          int right,
                          QVector<Step>& steps)
{
    if (left < right) {

        int mid = left + (right - left) / 2;

        mergeSort(array, left, mid, steps);
        mergeSort(array, mid + 1, right, steps);

        merge(array, left, mid, right, steps);
    }
}

void MergeSort::merge(QVector<int>& array,
                      int left,
                      int mid,
                      int right,
                      QVector<Step>& steps)
{
    QVector<int> temp;

    int i = left;
    int j = mid + 1;

    Step mergeStep;
    mergeStep.arrayState = array;
    mergeStep.activeIndices = {left, mid, right};
    mergeStep.type = StepType::Merge;
    mergeStep.description =
        QString("Объединяем части [%1..%2] и [%3..%4]")
            .arg(left)
            .arg(mid)
            .arg(mid + 1)
            .arg(right);

    steps.append(mergeStep);

    while (i <= mid && j <= right) {

        Step compareStep;
        compareStep.arrayState = array;
        compareStep.activeIndices = {i, j};
        compareStep.type = StepType::Compare;
        compareStep.description =
            QString("Сравниваем %1 и %2")
                .arg(array[i])
                .arg(array[j]);

        steps.append(compareStep);

        if (array[i] <= array[j]) {

            temp.append(array[i]);
            ++i;

        } else {

            temp.append(array[j]);
            ++j;
        }
    }

    while (i <= mid) {
        temp.append(array[i]);
        ++i;
    }

    while (j <= right) {
        temp.append(array[j]);
        ++j;
    }

    for (int k = 0; k < temp.size(); ++k) {

        array[left + k] = temp[k];

        Step writeStep;
        writeStep.arrayState = array;
        writeStep.activeIndices = {left + k};
        writeStep.type = StepType::Write;
        writeStep.description =
            QString("Записываем %1 в позицию %2")
                .arg(temp[k])
                .arg(left + k);

        steps.append(writeStep);
    }
}

#include "BubbleSort.h"
#include <algorithm>

QVector<Step> BubbleSort::execute(const QVector<int>& input)
{
    QVector<Step> steps;
    QVector<int> array = input;

    int n = array.size();

    for (int i = 0; i < n - 1; ++i) {
        bool swapped = false;

        for (int j = 0; j < n - i - 1; ++j) {

            Step compareStep;
            compareStep.arrayState = array;
            compareStep.activeIndices = {j, j + 1};
            compareStep.description =
                QString("Сравниваем %1 и %2")
                    .arg(array[j])
                    .arg(array[j + 1]);

            steps.append(compareStep);

            if (array[j] > array[j + 1]) {

                std::swap(array[j], array[j + 1]);
                swapped = true;

                Step swapStep;
                swapStep.arrayState = array;
                swapStep.activeIndices = {j, j + 1};
                swapStep.type = StepType::Swap;
                swapStep.description =
                    QString("Меняем %1 и %2 местами")
                        .arg(array[j])
                        .arg(array[j + 1]);

                steps.append(swapStep);
            }
        }

        if (!swapped) {
            break;
        }
    }

    Step finishedStep;
    finishedStep.arrayState = array;
    finishedStep.type = StepType::Finished;
    finishedStep.description = "Сортировка завершена";

    steps.append(finishedStep);

    return steps;
}

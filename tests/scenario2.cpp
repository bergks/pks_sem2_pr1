#include <QCoreApplication>
#include <QDebug>
#include "SortingController.h"
#include "ArrayModel.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    qDebug() << "=== Сценарий 2: Пошаговая демонстрация ===\n";

    ArrayModel model;
    SortingController controller(&model);

    model.setArray({5, 3, 1, 4, 2});
    controller.selectAlgorithm("Пузырёк");

    qDebug() << "Начальный массив:" << model.data();
    qDebug() << "Всего шагов:" << controller.totalSteps();

    for (int i = 0; i < controller.totalSteps(); i++) {
        controller.nextStep();
        qDebug() << "Шаг" << i+1 << ":" << model.data();
    }

    qDebug() << "\nИдем назад:";
    for (int i = controller.totalSteps(); i > 0; i--) {
        qDebug() << "Шаг" << i << ":" << model.data();
        controller.previousStep();
    }

    return 0;
}

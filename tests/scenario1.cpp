// tests/scenario1.cpp
#include <QCoreApplication>
#include <QDebug>
#include <algorithm>
#include <iostream>

#include "ArrayModel.h"
#include "SortingController.h"

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    qDebug() << "=== СЦЕНАРИЙ 1: Демонстрация всех алгоритмов сортировки ===\n";

    // Создаём модель данных (массив для сортировки)
    ArrayModel model;

    // Создаём контроллер, который управляет процессом сортировки
    SortingController controller(&model);

    // Тестовые данные
    QVector<int> originalArray = {64, 34, 25, 12, 22, 11, 90, 45, 33, 77};
    QStringList algorithms = {"Пузырёк", "Быстрая", "Слияние"};

    // Для каждого алгоритма демонстрируем пошаговую сортировку
    for (const QString& algoName : algorithms)
    {
        qDebug() << "\n--- Алгоритм:" << algoName << "---";

        // Устанавливаем исходный массив
        model.setArray(originalArray);

        // Выбираем алгоритм
        controller.selectAlgorithm(algoName);

        qDebug() << "Описание:" << controller.algorithmDescription();
        qDebug() << "Исходный массив:" << model.data();
        qDebug() << "Всего шагов:" << controller.totalSteps();

        // Сбрасываем к началу (на случай, если предыдущий алгоритм оставил шаги)
        while (controller.currentStep() > 0)
        {
            controller.previousStep();
        }

        // Выполняем сортировку по шагам
        int stepCount = 0;
        int maxSteps = 1000; // защита от бесконечного цикла

        while (controller.currentStep() < controller.totalSteps() && stepCount < maxSteps)
        {
            controller.nextStep();
            stepCount++;

            // Выводим состояние через каждые несколько шагов (чтобы не засорять вывод)
            if (stepCount % 2 == 0 || controller.currentStep() == controller.totalSteps())
            {
                qDebug() << "  Шаг" << controller.currentStep()
                    << "/" << controller.totalSteps()
                    << ":" << model.data()
                    << "-" << controller.stepDescription();
            }
        }

        // Финальное состояние
        QVector<int> result = model.data();
        bool isSorted = std::is_sorted(result.begin(), result.end());

        qDebug() << "Результат:" << result;
        qDebug() << "Отсортирован:" << (isSorted ? "ДА" : "НЕТ");

        // Проверяем корректность
        if (!isSorted)
        {
            qDebug() << "ОШИБКА: массив не отсортирован!";
            return 1;
        }

        // Проверяем, что элементы не потерялись
        QVector<int> sorted = originalArray;
        std::sort(sorted.begin(), sorted.end());
        if (result != sorted)
        {
            qDebug() << "ОШИБКА: результат не совпадает с ожидаемым!";
            qDebug() << "Ожидалось:" << sorted;
            return 1;
        }

        qDebug() << "Проверка пройдена успешно!";
    }

    // Дополнительно: тест навигации (шаги вперёд/назад)
    qDebug() << "\n--- Тест навигации ---";
    model.setArray({5, 3, 1, 4, 2});
    controller.selectAlgorithm("Пузырёк");

    qDebug() << "Начало:" << model.data();

    controller.nextStep();
    qDebug() << "После 1 шага:" << model.data();

    controller.nextStep();
    qDebug() << "После 2 шага:" << model.data();

    controller.previousStep();
    qDebug() << "После возврата на 1 шаг:" << model.data();

    controller.previousStep();
    qDebug() << "После возврата в начало:" << model.data();

    // Проверка, что нельзя уйти ниже нуля
    controller.previousStep();
    qDebug() << "Попытка уйти ниже 0:" << model.data() << "(шаг:" << controller.currentStep() << ")";

    if (controller.currentStep() != 0)
    {
        qDebug() << "ОШИБКА: шаг должен быть 0!";
        return 1;
    }

    qDebug() << "\n=== СЦЕНАРИЙ УСПЕШНО ЗАВЕРШЁН ===";
    return 0;
}

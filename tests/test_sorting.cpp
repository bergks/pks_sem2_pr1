#include <catch2/catch_all.hpp>

#include <algorithm>

#include "BubbleSort.h"
#include "QuickSort.h"
#include "MergeSort.h"
#include "SortingController.h"
#include "ArrayModel.h"

TEST_CASE("Sorting edge cases", "[sorting][edge]")
{
    auto [input, expected] =
        GENERATE(values<std::tuple<QVector<int>, QVector<int>>>({
            {{}, {}},
            {{1}, {1}},
            {{
                 7, 7, 7, 7, 7
             },{
                 7, 7, 7, 7, 7
             }},
            {{
                 1, 2, 3, 4, 5
             },{
                 1, 2, 3, 4, 5
             }},
            {{
                 3, 1, 2, 1, 3
             },{
                 1, 1, 2, 3, 3
             }},
            {{
                 30,29,28,27,26,25,24,23,22,21,
                 20,19,18,17,16,15,14,13,12,11,
                 10,9,8,7,6,5,4,3,2,1
             },{
                 1,2,3,4,5,6,7,8,9,10,
                 11,12,13,14,15,16,17,18,19,20,
                 21,22,23,24,25,26,27,28,29,30
             }}
        }));

    auto check = [&](auto& sort)
    {
        QVector<Step> steps = sort.execute(input);

        REQUIRE_FALSE(steps.isEmpty());

        CHECK(steps.last().arrayState == expected);
    };

    SECTION("BubbleSort")
    {
        BubbleSort sort;
        check(sort);
    }

    SECTION("QuickSort")
    {
        QuickSort sort;
        check(sort);
    }

    SECTION("MergeSort")
    {
        MergeSort sort;
        check(sort);
    }
}

TEST_CASE("Sorting invariants", "[invariants]")
{
    auto check = [](auto& sort)
    {
        for (int i = 0; i < 20; i++)
        {
            QVector<int> input;

            int size = rand() % 51;

            for (int j = 0; j < size; j++)
            {
                input.push_back(rand() % 100);
            }

            QVector<Step> steps = sort.execute(input);

            REQUIRE_FALSE(steps.isEmpty());

            QVector<int> result =
                steps.last().arrayState;

            QVector<int> expected = input;

            std::sort(
                expected.begin(),
                expected.end()
                );

            CHECK(result == expected);

            CHECK(std::is_sorted(
                result.begin(),
                result.end()
                ));
        }
    };

    SECTION("BubbleSort")
    {
        BubbleSort sort;
        check(sort);
    }

    SECTION("QuickSort")
    {
        QuickSort sort;
        check(sort);
    }

    SECTION("MergeSort")
    {
        MergeSort sort;
        check(sort);
    }
}

TEST_CASE("Controller selects algorithm", "[controller]")
{
    ArrayModel model;

    SortingController controller(&model);

    controller.selectAlgorithm("Пузырёк");

    CHECK(controller.currentAlgorithm() == "Пузырёк");
    CHECK_FALSE(controller.algorithmDescription().isEmpty());

    controller.selectAlgorithm("Быстрая");

    CHECK(controller.currentAlgorithm() == "Быстрая");

    controller.selectAlgorithm("Слияние");

    CHECK(controller.currentAlgorithm() == "Слияние");
}

TEST_CASE("Controller handles empty array", "[controller][edge]")
{
    auto algorithm =
        GENERATE(values<QString>({
            "Пузырёк",
            "Быстрая",
            "Слияние"
        }));

    ArrayModel model;

    model.setArray({});

    SortingController controller(&model);

    controller.selectAlgorithm(algorithm);

    CHECK(controller.totalSteps() > 0);

    while (controller.currentStep() < controller.totalSteps())
    {
        controller.nextStep();
    }

    CHECK(controller.currentStep()
          == controller.totalSteps());

    CHECK(model.data().isEmpty());
}

TEST_CASE("Controller executes next step", "[controller]")
{
    auto algorithm =
        GENERATE(values<QString>({
            "Пузырёк",
            "Быстрая",
            "Слияние"
        }));

    ArrayModel model;
    model.setArray({5, 3, 1, 4, 2});
    SortingController controller(&model);

    controller.selectAlgorithm(algorithm);
    CHECK(controller.currentStep() == 0);

    controller.nextStep();
    CHECK(controller.currentStep() == 1);

    while (controller.currentStep() < controller.totalSteps())
    {
        controller.nextStep();
    }
    CHECK(controller.currentStep() == controller.totalSteps());

    controller.nextStep();
    CHECK(controller.currentStep() == controller.totalSteps());
}

TEST_CASE("Controller completes sorting", "[controller]")
{
    auto algorithm =
        GENERATE(values<QString>({
            "Пузырёк",
            "Быстрая",
            "Слияние"
        }));

    ArrayModel model;
    model.setArray({5, 4, 3, 2, 1});
    SortingController controller(&model);

    controller.selectAlgorithm(algorithm);
    while (controller.currentStep() < controller.totalSteps())
    {
        controller.nextStep();
    }
    CHECK(controller.currentStep() == controller.totalSteps());

    QVector<int> result = model.data();

    CHECK(std::is_sorted(
        result.begin(),
        result.end()
        ));
}
TEST_CASE("Controller executes previous step", "[controller]")
{
    auto algorithm =
        GENERATE(values<QString>({
            "Пузырёк",
            "Быстрая",
            "Слияние"
        }));

    ArrayModel model;

    model.setArray({5, 3, 1, 4, 2});

    SortingController controller(&model);

    controller.selectAlgorithm(algorithm);

    controller.nextStep();
    controller.nextStep();

    REQUIRE(controller.currentStep() == 2);

    controller.previousStep();
    CHECK(controller.currentStep() == 1);
}

TEST_CASE("Controller previous step resets to start", "[controller]")
{
    auto algorithm =
        GENERATE(values<QString>({
            "Пузырёк",
            "Быстрая",
            "Слияние"
        }));

    ArrayModel model;

    model.setArray({5, 3, 1, 4, 2});

    SortingController controller(&model);

    controller.selectAlgorithm(algorithm);

    controller.nextStep();
    REQUIRE(controller.currentStep() == 1);

    controller.previousStep();
    CHECK(controller.currentStep() == 0);

    controller.previousStep();
    CHECK(controller.currentStep() == 0);
}

TEST_CASE("ArrayModel basic operations", "[arraymodel]")
{
    SECTION("Constructor creates empty model") {
        ArrayModel model;
        CHECK(model.data().isEmpty());
    }

    SECTION("setArray stores data") {
        ArrayModel model;
        model.setArray({1, 2, 3});
        CHECK(model.data() == QVector<int>{1, 2, 3});
    }

    SECTION("setArray with empty array") {
        ArrayModel model;
        model.setArray({});
        CHECK(model.data().isEmpty());
    }

    SECTION("setArray overwrites previous data") {
        ArrayModel model;
        model.setArray({1, 2, 3});
        model.setArray({4, 5});
        CHECK(model.data() == QVector<int>{4, 5});
    }

    SECTION("data returns copy not reference") {
        ArrayModel model;
        model.setArray({1, 2, 3});
        auto data = model.data();
        data[0] = 999;
        CHECK(model.data()[0] == 1);
    }
}

TEST_CASE("Scenario: Complete workflow demo", "[scenario]")
{
    ArrayModel model;
    SortingController controller(&model);

    QVector<QString> algorithms = {"Пузырёк", "Быстрая", "Слияние"};
    QVector<int> testData = {5, 3, 1, 4, 2};

    // Проверяем все три алгоритма
    for (const auto& algo : algorithms)
    {
        model.setArray(testData);
        controller.selectAlgorithm(algo);

        INFO("Algorithm: " << algo.toStdString());

        // Сбрасываем шаги
        while (controller.currentStep() > 0) {
            controller.previousStep();
        }

        // Выполняем сортировку
        int maxSteps = 1000; // защита от бесконечного цикла
        int steps = 0;
        while (controller.currentStep() < controller.totalSteps() && steps < maxSteps) {
            controller.nextStep();
            steps++;
        }

        QVector<int> result = model.data();
        INFO("Result size: " << result.size());
        CHECK(result.size() == testData.size());

        bool sorted = std::is_sorted(result.begin(), result.end());
        INFO("Is sorted: " << sorted);
        CHECK(sorted);
    }

    // Навигация
    model.setArray({3, 1, 2});
    controller.selectAlgorithm("Пузырёк");

    CHECK(controller.currentStep() == 0);

    controller.nextStep();
    CHECK(controller.currentStep() == 1);

    controller.previousStep();
    CHECK(controller.currentStep() == 0);
}

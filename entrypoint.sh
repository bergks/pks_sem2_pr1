#!/bin/bash

run_tests() {
    echo "======================================"
    echo "    ЗАПУСК UNIT-ТЕСТОВ (Catch2)"
    echo "======================================"
    /app/tests/test_sorting
    echo ""
    echo "======================================"
    echo "    ЗАПУСК СЦЕНАРИЯ 1"
    echo "======================================"
    /app/tests/scenario1_basic
    echo ""
    echo "======================================"
    echo "    ВСЕ ТЕСТЫ ПРОЙДЕНЫ"
    echo "======================================"
}

run_gui() {
    echo "Запуск GUI приложения..."
    /app/appsort_visualizer
}

case "${1}" in
    tests)
        run_tests
        ;;
    gui)
        run_gui
        ;;
    all)
        run_tests
        echo ""
        run_gui
        ;;
    *)
        echo "Usage: docker run ... [tests|gui|all]"
        echo "  tests  - run tests and scenarios"
        echo "  gui    - run GUI application"
        echo "  all    - run tests, then GUI"
        exit 1
        ;;
esac
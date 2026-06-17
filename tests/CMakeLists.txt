cmake_minimum_required(VERSION 3.16)

find_package(Qt6 REQUIRED Core)

# Указываем и исходную, и бинарную директорию
include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.7.1
)
FetchContent_MakeAvailable(Catch2)
# ============================================
# 1. Основной исполняемый файл с unit-тестами
# ============================================
add_executable(test_sorting
    test_sorting.cpp
    ../src/BubbleSort.cpp
    ../src/QuickSort.cpp
    ../src/MergeSort.cpp
    ../src/SortingController.cpp
    ../src/ArrayModel.cpp
)

target_link_libraries(test_sorting PRIVATE
    Catch2::Catch2WithMain
    Qt6::Core
)

target_include_directories(test_sorting PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
)

set_target_properties(test_sorting PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# ============================================
# 2. Сценарий 1 - отдельная программа
# ============================================
add_executable(scenario1_basic
    scenario1.cpp
    ../src/BubbleSort.cpp
    ../src/QuickSort.cpp
    ../src/MergeSort.cpp
    ../src/SortingController.cpp
    ../src/ArrayModel.cpp
)

target_link_libraries(scenario1_basic PRIVATE
    Qt6::Core
)

target_include_directories(scenario1_basic PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
)

set_target_properties(scenario1_basic PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# ============================================
# 3. Сценарий 2 - отдельная программа
# ============================================
add_executable(scenario2_interactive
    scenario2.cpp
    ../src/BubbleSort.cpp
    ../src/QuickSort.cpp
    ../src/MergeSort.cpp
    ../src/SortingController.cpp
    ../src/ArrayModel.cpp
)

target_link_libraries(scenario2_interactive PRIVATE
    Qt6::Core
)

target_include_directories(scenario2_interactive PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
)

set_target_properties(scenario2_interactive PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

enable_testing()
add_test(NAME scenario1_basic COMMAND scenario1_basic)
add_test(NAME scenario2_interactive COMMAND scenario2_interactive)

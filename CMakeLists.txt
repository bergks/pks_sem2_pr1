cmake_minimum_required(VERSION 3.16)

project(sort_visualizer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick QuickControls2)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appsort_visualizer
    src/main.cpp
    src/ArrayModel.cpp
    src/SortingController.cpp
    src/BubbleSort.cpp
    src/QuickSort.cpp
    src/MergeSort.cpp
    src/ArrayModel.h
    src/SortingController.h
    src/SortingStrategy.h
    src/BubbleSort.h
    src/QuickSort.h
    src/MergeSort.h
)

qt_add_qml_module(appsort_visualizer
    VERSION 1.0
    URI sort_visualizer
    QML_FILES
        qml/Main.qml
    SOURCES
        src/SortingStrategy.h
        src/ArrayModel.cpp
        src/BubbleSort.cpp
        src/MergeSort.cpp
        src/QuickSort.cpp
        src/SortingController.cpp
)

target_include_directories(appsort_visualizer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(appsort_visualizer
    PRIVATE Qt6::Quick Qt6::QuickControls2
)

include(GNUInstallDirs)
install(TARGETS appsort_visualizer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

add_subdirectory(tests)

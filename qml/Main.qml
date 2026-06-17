import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import sort_visualizer

ApplicationWindow {
    id: mainWindow
    width: 900
    height: 680
    visible: true
    title: "SortVis"
    color: "#1a1a2e"

    // ──────────────────────────────────────────────
    // Выезжающая панель управления (Drawer)
    // ──────────────────────────────────────────────
    Drawer {
        id: controlPanel
        width: 280
        height: parent.height
        edge: Qt.LeftEdge
        modal: false
        interactive: true
        dragMargin: 30

        background: Rectangle {
            color: "#16213e"
            border.color: "#0f3460"
            border.width: 1
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 16
            spacing: 12

            // Заголовок панели
            Text {
                text: "Управление"
                font.pixelSize: 18
                font.weight: Font.DemiBold
                color: "#e94560"
                Layout.alignment: Qt.AlignHCenter
            }

            Rectangle { Layout.fillWidth: true; height: 1; color: "#0f3460" }

            // Выбор алгоритма
            Text {
                text: "Алгоритм"
                font.pixelSize: 12
                color: "#a0a0b0"
            }
            ComboBox {
                id: algorithmSelector
                Layout.fillWidth: true
                model: ["Пузырёк", "Быстрая", "Слияние"]
                onCurrentTextChanged: controller.selectAlgorithm(currentText)
                background: Rectangle {
                    color: "#1a1a2e"
                    border.color: "#0f3460"
                    radius: 6
                }
                contentItem: Text {
                    text: algorithmSelector.currentText
                    color: "#eaeaea"
                    verticalAlignment: Text.AlignVCenter
                    leftPadding: 10
                }
            }

            // O-нотация
            Rectangle {
                Layout.fillWidth: true
                height: 28
                radius: 4
                color: "#0f3460"
                Text {
                    anchors.centerIn: parent
                    text: controller.complexity || "O(?)"
                    font.pixelSize: 13
                    font.weight: Font.Medium
                    color: "#e94560"
                }
            }

            Rectangle { Layout.fillWidth: true; height: 1; color: "#0f3460" }

            // Размер массива
            Text {
                text: "Размер массива"
                font.pixelSize: 12
                color: "#a0a0b0"
            }
            RowLayout {
                SpinBox {
                    id: sizeInput
                    from: 5; to: 30; value: 15
                    editable: true
                    Layout.fillWidth: true
                    background: Rectangle {
                        color: "#1a1a2e"
                        border.color: "#0f3460"
                        radius: 6
                    }
                    contentItem: Text {
                        text: sizeInput.value
                        color: "#eaeaea"
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }
                }
                Button {
                    text: "↻"
                    onClicked: {
                        controller.stop()
                        arrayModel.generateRandom(sizeInput.value)
                    }
                    background: Rectangle {
                        color: "#e94560"
                        radius: 6
                    }
                    contentItem: Text {
                        text: "↻"
                        font.pixelSize: 16
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }

            Rectangle { Layout.fillWidth: true; height: 1; color: "#0f3460" }

            // Кнопки управления
            RowLayout {
                Button {
                    text: controller.isRunning && !controller.isPaused ? "⏸" : "▶"
                    Layout.fillWidth: true
                    onClicked: {
                        if (controller.isRunning && !controller.isPaused) controller.pause()
                        else controller.start()
                    }
                    background: Rectangle { color: "#e94560"; radius: 6 }
                    contentItem: Text {
                        text: parent.text
                        font.pixelSize: 18; color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
                Button {
                    text: "⏹"
                    Layout.fillWidth: true
                    onClicked: controller.stop()
                    background: Rectangle { color: "#533483"; radius: 6 }
                    contentItem: Text {
                        text: "⏹"; font.pixelSize: 18; color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }

            RowLayout {
                Button {
                    text: "◀"
                    Layout.fillWidth: true
                    onClicked: controller.previousStep()
                    background: Rectangle { color: "#0f3460"; radius: 6 }
                    contentItem: Text {
                        text: "◀"; font.pixelSize: 14; color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
                Button {
                    text: "▶"
                    Layout.fillWidth: true
                    onClicked: controller.nextStep()
                    background: Rectangle { color: "#0f3460"; radius: 6 }
                    contentItem: Text {
                        text: "▶"; font.pixelSize: 14; color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }

            Rectangle { Layout.fillWidth: true; height: 1; color: "#0f3460" }

            // Скорость
            // Скорость
            Text {
                text: "Скорость: " + controller.speed + " мс"
                font.pixelSize: 12
                color: "#a0a0b0"
            }

            Slider {
                id: speedSlider
                Layout.fillWidth: true
                from: 10
                to: 1000

                // Начальное значение
                Component.onCompleted: {
                    speedSlider.value = controller.speed
                }

                // Следим за изменением скорости в контроллере
                Connections {
                    target: controller
                    function onSpeedChanged() {
                        speedSlider.value = controller.speed
                    }
                }

                // Отправляем новое значение при отпускании
                onPressedChanged: {
                    if (!pressed) {
                        controller.setSpeed(Math.round(speedSlider.value))
                    }
                }

                // Минимальный стиль, который работает с Fusion
                background: Rectangle {
                    color: "#0f3460"
                    radius: 2
                    height: 4
                    y: (parent.height - height) / 2
                }
            }

            Item { Layout.fillHeight: true }
        }
    }

    // ──────────────────────────────────────────────
    // Основная область
    // ──────────────────────────────────────────────
    ColumnLayout {
        anchors.fill: parent
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.topMargin: 12
        anchors.bottomMargin: 12
        spacing: 8

        // Верхняя строка: бургер + заголовок
        RowLayout {
            Layout.fillWidth: true
            Button {
                text: "☰"
                onClicked: controlPanel.open()
                background: Rectangle { color: "transparent" }
                contentItem: Text {
                    text: "☰"; font.pixelSize: 22; color: "#e94560"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
            Text {
                text: "SortVis"
                font.pixelSize: 22
                font.weight: Font.Bold
                color: "#eaeaea"
            }
            Text {
                text: "· визуализатор алгоритмов"
                font.pixelSize: 12
                color: "#a0a0b0"
            }
            Item { Layout.fillWidth: true }

            // Счётчик шагов в шапке
            Text {
                text: "Шаг " + controller.currentStep + " / " + controller.totalSteps
                font.pixelSize: 13
                color: "#a0a0b0"
            }
        }

        // Прогресс-бар
        ProgressBar {
            Layout.fillWidth: true
            value: controller.totalSteps > 0 ? controller.currentStep / Number(controller.totalSteps) : 0
            background: Rectangle {
                color: "#0f3460"
                radius: 3
            }
            contentItem: Rectangle {
                color: "#e94560"
                radius: 3
            }
        }

        // ──────────────────────────────────────────
        // Визуализация столбцов
        // ──────────────────────────────────────────
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#16213e"
            radius: 10
            border.color: "#0f3460"
            border.width: 1

            Row {
                id: barsContainer
                anchors {
                    fill: parent
                    margins: 15
                    bottomMargin: 40
                }
                spacing: 3

                Repeater {
                    model: arrayModel.data

                    Rectangle {
                        width: arrayModel.size > 0
                               ? (barsContainer.width - 3 * (arrayModel.size - 1)) / arrayModel.size
                               : 0
                        height: arrayModel.size > 0
                                ? (modelData / (arrayModel.size * 2.0)) * barsContainer.height
                                : 0
                        anchors.bottom: parent.bottom
                        radius: 3

                        // Градиентная заливка
                        gradient: Gradient {
                            GradientStop {
                                position: 0.0
                                color: {
                                    if (controller.pivotIndex === index) return "#ffc060"
                                    if (controller.activeIndexes.includes(index)) {
                                        if (controller.currentStepType === 1) return "#ff6b6b"
                                        if (controller.currentStepType === 2) return "#4dabf7"
                                        if (controller.currentStepType === 4) return "#a8e6cf"
                                        if (controller.currentStepType === 5) return "#c77dff"
                                        return "#ff6b6b"
                                    }
                                    return "#e94560"
                                }
                            }
                            GradientStop {
                                position: 1.0
                                color: {
                                    if (controller.pivotIndex === index) return "#e6a030"
                                    if (controller.activeIndexes.includes(index)) {
                                        if (controller.currentStepType === 1) return "#cc4444"
                                        if (controller.currentStepType === 2) return "#2d8ac7"
                                        if (controller.currentStepType === 4) return "#7ecba0"
                                        if (controller.currentStepType === 5) return "#9d4ddd"
                                        return "#cc4444"
                                    }
                                    return "#0f3460"
                                }
                            }
                        }

                        Behavior on height {
                            NumberAnimation { duration: 150 }
                        }

                        Behavior on gradient {
                            ColorAnimation { duration: 100 }
                        }

                        Text {
                            anchors {
                                bottom: parent.top
                                bottomMargin: 4
                                horizontalCenter: parent.horizontalCenter
                            }
                            text: modelData
                            font.pixelSize: 10
                            color: "#a0a0b0"
                        }
                    }
                }
            }
        }

        // ──────────────────────────────────────────
        // Нижняя информационная панель
        // ──────────────────────────────────────────
        Rectangle {
            Layout.fillWidth: true
            height: 50
            color: "#16213e"
            radius: 8
            border.color: "#0f3460"
            border.width: 1

            Text {
                anchors.centerIn: parent
                width: parent.width - 20
                text: controller.stepDescription || controller.algorithmDescription
                font.pixelSize: 13
                color: "#c0c0d0"
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter
            }
        }
    }

    // ──────────────────────────────────────────────
    // Диалог завершения
    // ──────────────────────────────────────────────
    Dialog {
        id: finishDialog
        title: "Готово!"
        modal: true
        anchors.centerIn: parent
        background: Rectangle {
            color: "#16213e"
            border.color: "#e94560"
            radius: 10
        }
        header: Text {
            text: "Готово!"
            font.pixelSize: 18
            color: "#e94560"
            padding: 16
        }
        ColumnLayout {
            Text {
                text: "Сортировка успешно завершена!"
                font.pixelSize: 14
                color: "#eaeaea"
            }
            Button {
                text: "OK"
                onClicked: finishDialog.close()
                Layout.alignment: Qt.AlignHCenter
                background: Rectangle { color: "#e94560"; radius: 6 }
            }
        }
    }

    // ──────────────────────────────────────────────
    // Connections
    // ──────────────────────────────────────────────
    Connections {
        target: controller
        function onSortingFinished() { finishDialog.open() }
    }

    Component.onCompleted: {
        arrayModel.generateRandom(15)
        controller.selectAlgorithm("Пузырёк")
    }
}

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QtGlobal>

#include "ArrayModel.h"
#include "SortingController.h"

int main(int argc, char *argv[])
{
    QQuickStyle::setStyle("Fusion");

    QGuiApplication app(argc, argv);

    ArrayModel arrayModel;

    SortingController controller(&arrayModel);

    controller.selectAlgorithm("Пузырёк");

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty(
        "arrayModel", &arrayModel);

    engine.rootContext()->setContextProperty(
        "controller", &controller);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
    engine.loadFromModule("sort_visualizer", "Main");
#else
    engine.load(QUrl::fromLocalFile("/app/qml/Main.qml"));
#endif

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}

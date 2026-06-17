#include "mainwindow.h"
#include <QApplication>
#include <QQuickStyle>
#include <QtGlobal>

int main(int argc, char *argv[])
{
    qputenv("QT_QUICK_CONTROLS_STYLE", "Fusion");

    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}

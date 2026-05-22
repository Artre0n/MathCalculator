#include <QApplication>
#include <QStyleHints>
#include "Mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QGuiApplication::styleHints()->setColorScheme(Qt::ColorScheme::Light);

    MainWindow w;
    w.show();
    return app.exec();
}
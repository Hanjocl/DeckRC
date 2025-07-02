#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <iostream>
#include "test.h"
// #include "inputController.h"


int main(int argc, char *argv[]) {
    int result = adder(2, 3);  // Example usage
    std::cout << "Adder result with STD: " << result << std::endl;
    std::cout.flush();
    qDebug() << "Adder result with qDebug:" << result;

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/DeckRC/src/UI/Main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}

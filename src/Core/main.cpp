#define SDL_MAIN_HANDLED

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <iostream>
#include "test.h"
#include "inputControllerModel.h"

int main(int argc, char *argv[]) {
    std::cout << "Launching DeckRC " << std::endl;
    // TESTING
    int result = adder(2, 3);  // Example usage
    std::cout << "Adder result with STD: " << result << std::endl;
    qDebug() << "Adder result with qDebug:" << result;

    
    QGuiApplication app(argc, argv);

    InputControllerModel inputController;

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/DeckRC/src/UI/Main.qml"));

    // Create the singleton instance
    inputController.setCurrentValue(1600);

    std::cout << "Current Value: " << inputController.currentValue() << std::endl;

    engine.rootContext()->setContextProperty("InputController", &inputController);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    int result_app = app.exec();

    // Do stuff after shutting down app

    std::cout << "Closing App... " << std::endl;
    return result_app;
}

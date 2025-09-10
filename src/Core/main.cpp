#define SDL_MAIN_HANDLED

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <iostream>
#include <SDL.h>

#include "inputControllerModel.h"
#include "ControllerModel/controllerModel.h"

#include "crsf.h"

int main(int argc, char *argv[]) {
    std::cout << "Launching DeckRC " << std::endl;
    
    // Start CRSF Manager
    CRSF crsfManager(CRSF_ADDRESS_RADIO_TRANSMITTER);
    std::vector<int> new_channels = {1500, 1500, 1500, 1500, 1500, 1900, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500};
    crsfManager.begin("COM3"); // TO-DO: Make this configurable from UI


    // Start QML
    QGuiApplication app(argc, argv);

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "SDL_InitSubSystem Error: " << SDL_GetError() << std::endl;
        return false;
    }
    InputControllerModel inputController;

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/DeckRC/src/UI/Main.qml"));


    // Controller *controller = new Controller;
    // controller->start();  // Start the worker thread
    // engine.rootContext()->setContextProperty("controller", controller);
    
    engine.rootContext()->setContextProperty("InputController", &inputController);
    
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);
        
    //inputController.startPolling(50);
    int result_app = app.exec();

    // Do stuff after shutting down app
    crsfManager.stop();
    SDL_Quit();

    std::cout << "Closing App... " << std::endl;
    return result_app;
}

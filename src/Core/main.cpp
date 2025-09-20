#define SDL_MAIN_HANDLED

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <iostream>
#include <SDL.h>

#include "crsf.h"
#include <QmlCrsfApi.h>

#include "inputController.h"
#include <QmlControllerApi.h>

int main(int argc, char *argv[]) {
    std::cout << "Launching DeckRC " << std::endl;
    
    // Create instances CRSF and QML_CRSF
    CRSF crsfManager(CRSF_ADDRESS_RADIO_TRANSMITTER); // Creates a remote controller
    QmlCrsfAPI qmlCrsfApi(crsfManager);
    std::vector<int> new_channels = {1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500};
    crsfManager.setChannels(new_channels);

    
    
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "SDL_InitSubSystem Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    Inputs sdlController(16); // Create controller with 16 channels
    QmlControllerApi inputController(sdlController);
    inputController.setDebug(false);
    inputController.setChannelsCallback([&crsfManager](const std::vector<ChannelDataType>& channels) {
        crsfManager.setChannels(channels);
    });

    

    // Start QML
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/DeckRC/src/UI/Main.qml"));
    
    engine.rootContext()->setContextProperty("InputController", &inputController);
    engine.rootContext()->setContextProperty("CsrfApi", &qmlCrsfApi);
    engine.rootContext()->setContextProperty("SdlController", &inputController);
    
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

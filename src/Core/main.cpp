#define SDL_MAIN_HANDLED

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <iostream>
#include <SDL.h>

// CRSF Controller
#include "crsf.h"
#include <QmlCrsfApi.h>
// SDL Controller
#include "inputController.h"
#include <QmlControllerApi.h>
//OpenIPC Controller
#include <QQuickWindow>
#include "QQuickRealTimePlayer.h"
#include <QmlNativeAPI.h>


int main(int argc, char *argv[]) {
    std::cout << "Launching DeckRC " << std::endl;
    
    // CRSF Transmitter
    CRSF crsfManager(CRSF_ADDRESS_RADIO_TRANSMITTER); // Creates a remote controller
    QmlCrsfAPI qmlCrsfApi(crsfManager);
    std::vector<int> new_channels = {1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000};
    crsfManager.setChannels(new_channels);

    
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "SDL_InitSubSystem Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // SDL Controller
    Inputs sdlController(16); // Create controller with 16 channels
    QmlControllerApi inputController(sdlController);
    inputController.setDebug(false);
    inputController.setChannelsCallback([&crsfManager](const std::vector<ChannelDataType>& channels) {
        crsfManager.setChannels(channels);
    });

    // OpenIPC
    QCoreApplication::setAttribute (Qt::AA_UseDesktopOpenGL);    
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);
    QQuickWindow::setSceneGraphBackend("opengl");
    

    // Start QML
    QApplication app(argc, argv);
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/DeckRC/src/UI/Main.qml"));
    
    engine.rootContext()->setContextProperty("InputController", &inputController);
    engine.rootContext()->setContextProperty("CsrfApi", &qmlCrsfApi);
    engine.rootContext()->setContextProperty("SdlController", &inputController);
    
    qmlRegisterType<QQuickRealTimePlayer>("realTimePlayer", 1, 0, "QQuickRealTimePlayer");  
    auto &qmlNativeApi = QmlNativeAPI::Instance();
    engine.rootContext()->setContextProperty("NativeApi", &qmlNativeApi);
    
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
    qmlNativeApi.Stop();
    crsfManager.stop();
    SDL_Quit();

    std::cout << "Closing App... " << std::endl;
    return result_app;
}

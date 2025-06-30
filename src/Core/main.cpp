#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QDebug>

int main(int argc, char *argv[])
{
    qDebug() << "Hello Q!";
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    
    const QUrl url(QStringLiteral("qrc:/DeckRC/src/UI/Screens/Main.qml"));
    qDebug() << "Trying to load:" << url;

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}

#include "controllerModel.h"

Controller::Controller(QObject *parent) : QObject(parent), _worker(new Worker), workerThread(new QThread) {
    _worker->moveToThread(workerThread);
    
    connect(workerThread, &QThread::started, _worker, &Worker::onThreadStarted);
    connect(workerThread, &QThread::finished, _worker, &QObject::deleteLater);
}

Controller::~Controller() {
    workerThread->quit();
    workerThread->wait();
    _worker->deleteLater();
    workerThread->deleteLater();
}

void Controller::start() {
    workerThread->start();
}

QObject* Controller::worker() const {
    return _worker;
}
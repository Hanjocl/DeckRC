#include "worker.h"

#include <iostream>
#include <QThread>
#include <QDebug>

Worker::Worker(QObject *parent, int amount_channels) : QObject(parent), inputs(16), channels(16) {
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Worker::updateInputs);
}

void Worker::onThreadStarted() {
    qDebug() << "Worker running in thread:" << QThread::currentThread();

    inputs.addAxis(0, 0, 0, 500);
    inputs.addAxis(1, 1, 0, 500);
    inputs.addAxis(2, 2, 0, 500);
    inputs.addAxis(3, 3, 0, 500);
    inputs.addAxis(4, 4, 0, 500);
    inputs.addAxis(5, 5, 0, 500);
    inputs.addToggle(6, SDLK_f, -400);
    inputs.addToggle(7, SDLK_a, 400);
    inputs.addToggle(8, 0, 0, 400);

    startPolling(50);  // Start polling at 50Hz
}

void Worker::updateInputs() {
    inputs.cycle(channels);
    printChannels(channels);

    const auto channels = inputs.getChannels();
    // if (!channels.empty())
    //     setCurrentValue(channels.at(0));

    emit channelValuesChanged(); // Notify QML to refresh the ListView
}

void Worker::startPolling(int intervalHz) {
    SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
    int intervalMs = 1000 / intervalHz;
    if (!timer->isActive()) {
        timer->start(intervalMs);
    } else {
        timer->setInterval(intervalMs);
    }
}

void Worker::setPollingInterval(int intervalHz) {
    int intervalMs = 1000 / intervalHz;
    if (timer->isActive())
        timer->setInterval(intervalMs);
}

void Worker::stopPolling() {
    timer->stop();
}


// DEBUGGING
void Worker::printChannels(const std::vector<ChannelDataType>& channels) {
    std::cout << "Worker Channels: ";
    for (size_t i = 0; i < channels.size(); ++i) {
        std::cout << channels[i];
        if (i != channels.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
}

#pragma once
#include <QObject>
#include <QTimer>

#include "inputController.h"

class Worker : public QObject {
    Q_OBJECT
public:
    Worker(QObject *parent = nullptr, int amount_channels = 16);
    void startPolling(int intervalHz = 50);
    void setPollingInterval(int intervalHz);
    void stopPolling();

signals:
    void channelValuesChanged();

public slots:
    void onThreadStarted();  // Called from QThread::started()
    
private:
    void updateInputs();
    Inputs inputs;
    std::vector<ChannelDataType> channels;
    QTimer* timer;

    // DEBUGGING
    void printChannels(const std::vector<ChannelDataType>& channels);
};

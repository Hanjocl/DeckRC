#ifndef INPUTCONTROLLERMODEL_H
#define INPUTCONTROLLERMODEL_H

#include <vector>
#include <iostream>
#include <QObject>
#include <QTimer>
#include <QVariant>

#include "inputController.h"

class InputControllerModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(int currentValue READ currentValue WRITE setCurrentValue NOTIFY currentValueChanged)
    Q_PROPERTY(QVariantList channelValues READ channelValues NOTIFY channelValuesChanged)

public:
    explicit InputControllerModel(QObject *parent = nullptr, int amount_channels = 16);

    // QTIMER
    Q_INVOKABLE void startPolling(int intervalHz = 50);
    Q_INVOKABLE void setPollingInterval(int intervalHz);
    Q_INVOKABLE void stopPolling();

    // CHANNELS
    QVariantList channelValues() const;
    
    //Testing
    int currentValue() const;
    void setCurrentValue(int value);

signals:
    void currentValueChanged();
    void channelValuesChanged();

private:
    // Library specific
    void updateInputs();
    Inputs m_inputs;
    std::vector<ChannelDataType> m_channels;
    
    // QML Timer
    QTimer m_timer;

    // Debugging
    void printChannels(const std::vector<ChannelDataType>& channels);

    // Random testing
    int m_currentValue;
};

#endif // INPUTCONTROLLERMODEL_H
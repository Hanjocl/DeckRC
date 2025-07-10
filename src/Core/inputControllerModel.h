#ifndef INPUTCONTROLLERMODEL_H
#define INPUTCONTROLLERMODEL_H

#include <vector>
#include <QObject>
#include <QTimer>
#include "inputController.h"

class InputControllerModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(int currentValue READ currentValue WRITE setCurrentValue NOTIFY currentValueChanged)

public:
    explicit InputControllerModel(QObject *parent = nullptr, int amount_channels = 16);

    // QTIMER
    Q_INVOKABLE void startPolling(int intervalMs = 100);
    Q_INVOKABLE void setPollingInterval(int intervalMs);
    Q_INVOKABLE void stopPolling();
    
    //Testing
    int currentValue() const;
    void setCurrentValue(int value);

signals:
    void currentValueChanged();
    void channelValuesChanged();

private:
    void updateInputs();
    // Inputs m_inputs;
    // std::vector<ChannelDataType> m_channels;
    
    QTimer m_timer;
    int m_currentValue;
};

#endif // INPUTCONTROLLERMODEL_H
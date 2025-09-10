#ifndef INPUTCONTROLLERMODEL_H
#define INPUTCONTROLLERMODEL_H

#include <vector>
#include <iostream>
#include <QObject>
#include <QTimer>
#include <QVariant>

#include <SDL_keycode.h>

#include "inputController.h"
#include "inputTypes.h"

class InputControllerModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVariantList channelValues READ channelValues NOTIFY channelValuesChanged)

public:
    explicit InputControllerModel(QObject *parent = nullptr, int amount_channels = 16);
    ~InputControllerModel();

    bool init();

    // QTIMER
    Q_INVOKABLE void startPolling(int intervalHz = 50);
    Q_INVOKABLE void setPollingInterval(int intervalHz);
    Q_INVOKABLE void stopPolling();

    // CHANNELS
    QVariantList channelValues() const;

    // UI COMMANDS
    // void GetInput();
    // void ApplyConfigToChannel(int channel, int value, ChannelModes mode, SDL_Keycode key = nullptr, int JoystickID = nullptr, int JoystickInput);
    
    Q_INVOKABLE void stopScanning() { scanning = false; }
    Q_INVOKABLE void injectKey(int qtKey, const QString& text);

signals:
    void channelValuesChanged();
    void keyEvent(QString key, int qtKeyCode);

private:
    // Library specific
    void updateInputs();
    Inputs m_inputs;
    std::vector<ChannelDataType> m_channels;
    
    // Input Detection
    bool scanning = true;
    
    // QML Timer
    QTimer m_timer;

    // Debugging
    void printChannels(const std::vector<ChannelDataType>& channels);
};

#endif // INPUTCONTROLLERMODEL_H
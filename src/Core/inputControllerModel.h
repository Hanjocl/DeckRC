#ifndef INPUTCONTROLLERMODEL_H
#define INPUTCONTROLLERMODEL_H

#include <vector>
#include <iostream>
#include <QObject>
#include <QTimer>
#include <QVariant>

#include "inputController.h"
#include "inputTypes.h"

class InputControllerModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(int currentValue READ currentValue WRITE setCurrentValue NOTIFY currentValueChanged)
    Q_PROPERTY(QVariantList channelValues READ channelValues NOTIFY channelValuesChanged)

public:
    explicit InputControllerModel(QObject *parent = nullptr, int amount_channels = 16);

    enum class ChannelModes {
        KEYBOARD_KEY_HOLD,
        KEYBOARD_KEY_RELEASE,
        KEYBOARD_KEY_INCREMENT,
        KEYBOARD_KEY_TOGGLE,
        KEYBOARD_KEY_TOGGLE_SYMETRIC,
        KEYBOARD_KEY_TAP,

        JOYSTICK_BUTTON_HOLD,
        JOYSTICK_BUTTON_RELEASE,
        JOYSTICK_BUTTON_INCREMENT,
        JOYSTICK_BUTTON_TOGGLE,
        JOYSTICK_BUTTON_TOGGLE_SYMETRIC,
        JOYSTICK_BUTTON_TAP,

        JOYSTICK_AXIS,
        JOYSTICK_AXIS_HOLD,
        JOYSTICK_AXIS_TOGGLE,
        JOYSTICK_AXIS_TOGGLE_SYMETRIC,
        JOYSTICK_AXIS_INCREMENT,
    };
    Q_ENUM(ChannelModes)

    // QTIMER
    Q_INVOKABLE void startPolling(int intervalHz = 50);
    Q_INVOKABLE void setPollingInterval(int intervalHz);
    Q_INVOKABLE void stopPolling();

    // CHANNELS
    QVariantList channelValues() const;

    // UI COMMANDS
    // void GetInput();
    // void ApplyConfigToChannel(int channel, int value, ChannelModes mode, SDL_keycode key = nullptr, int JoystickID = nullptr, int JoystickInput)
    Q_INVOKABLE void GetInput();
    Q_INVOKABLE void stopScanning() { scanning = false; }
    InputEvent detectFirstInputEvent();

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
    
    // Input Detection
    bool scanning = true;
    
    // QML Timer
    QTimer m_timer;

    // Debugging
    void printChannels(const std::vector<ChannelDataType>& channels);

    // Random testing
    int m_currentValue;
};

#endif // INPUTCONTROLLERMODEL_H
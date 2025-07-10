#include "inputcontrollermodel.h"
#include <QVariantList>

InputControllerModel::InputControllerModel(QObject *parent, int amount_channels) 
: QObject(parent), m_currentValue(0) {            // TODO: FIX creating input -> Maybe not doing it in the constructor?
    //connect(&m_timer, &QTimer::timeout, this, &InputControllerModel::updateInputs);
}

// void InputControllerModel::updateInputs() {
//     m_inputs.cycle();

//     const auto channels = m_inputs.getChannels();
//     if (!channels.empty())
//         setCurrentValue(channels.at(0));

//     emit channelValuesChanged(); // Notify QML to refresh the ListView
// }


void InputControllerModel::startPolling(int intervalMs) {
    if (!m_timer.isActive()) {
        m_timer.start(intervalMs);
    } else {
        m_timer.setInterval(intervalMs);
    }
}

void InputControllerModel::setPollingInterval(int intervalMs) {
    if (m_timer.isActive())
        m_timer.setInterval(intervalMs);
}

void InputControllerModel::stopPolling() {
    m_timer.stop();
}


// TEST
int InputControllerModel::currentValue() const {
    return m_currentValue;
}

void InputControllerModel::setCurrentValue(int value) {
    if (m_currentValue != value) {
        m_currentValue = value;
        emit currentValueChanged();
    }
}
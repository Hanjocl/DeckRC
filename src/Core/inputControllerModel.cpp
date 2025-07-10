#include "inputcontrollermodel.h"
#include <QVariantList>
#include <iostream>

InputControllerModel::InputControllerModel(QObject *parent, int amount_channels) 
: QObject(parent), m_currentValue(0), m_inputs(16), m_channels(16) {
    std::cout << "Setting up Input Controls " << std::endl;
    m_inputs.addToggle(0, SDLK_a, 100);
    m_inputs.addToggle(1, SDLK_a, 100);
    m_inputs.addToggle(2, 0, 0, 100);
    connect(&m_timer, &QTimer::timeout, this, &InputControllerModel::updateInputs);
}

void InputControllerModel::updateInputs() {
    m_inputs.cycle(m_channels);
    printChannels(m_channels);

    const auto channels = m_inputs.getChannels();
    if (!channels.empty())
        setCurrentValue(channels.at(0));

    emit channelValuesChanged(); // Notify QML to refresh the ListView
}


void InputControllerModel::startPolling(int intervalHz) {
    int intervalMs = 1000 / intervalHz;
    if (!m_timer.isActive()) {
        m_timer.start(intervalMs);
    } else {
        m_timer.setInterval(intervalMs);
    }
}

void InputControllerModel::setPollingInterval(int intervalHz) {
    int intervalMs = 1000 / intervalHz;
    if (m_timer.isActive())
        m_timer.setInterval(intervalMs);
}

void InputControllerModel::stopPolling() {
    m_timer.stop();
}

QVariantList InputControllerModel::channelValues() const {
    QVariantList list;
    for (const auto& val : m_inputs.getChannels()) {
        list.append(QVariant::fromValue(val));  // assuming ChannelDataType can be converted to QVariant
    }
    return list;
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

void InputControllerModel::printChannels(const std::vector<ChannelDataType>& channels) {
    std::cout << "Channels: ";
    for (size_t i = 0; i < channels.size(); ++i) {
        std::cout << channels[i];
        if (i != channels.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
}
#include "inputcontrollermodel.h"

#include <QVariantList>
#include <iostream>
#include <SDL.h>

InputControllerModel::InputControllerModel(QObject *parent, int amount_channels) 
: QObject(parent), m_currentValue(0), m_inputs(16), m_channels(16) {
    std::cout << "Setting up Input Controls " << std::endl;
    //m_inputs.addAxis(0, 0, 0, 500);
    m_inputs.addAxis(1, 1, 0, 500);
    m_inputs.addAxis(2, 2, 0, 500);
    m_inputs.addAxis(3, 3, 0, 500);
    m_inputs.addAxis(4, 4, 0, 500);
    m_inputs.addAxis(5, 5, 0, 500);
    m_inputs.addToggle(0, SDLK_f, -400);
    m_inputs.addToggle(0, SDLK_a, 400);
    m_inputs.addToggle(8, 0, 0, 400);
    connect(&m_timer, &QTimer::timeout, this, &InputControllerModel::updateInputs);
}

InputControllerModel::~InputControllerModel() {
    std::cout << "Cleaning up Input Controls " << std::endl;
    stopPolling();
    SDL_Quit();
}

bool InputControllerModel::init() {

    // Initialize any additional resources or settings if needed
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "SDL_InitSubSystem Error: " << SDL_GetError() << std::endl;
        return false;
    }
    std::cout << "InputControllerModel initialized." << std::endl;
    return true;

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
    SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
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

// DEBUGGING
void InputControllerModel::printChannels(const std::vector<ChannelDataType>& channels) {
    std::cout << "Channels: ";
    for (size_t i = 0; i < channels.size(); ++i) {
        std::cout << channels[i];
        if (i != channels.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
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


void InputControllerModel::injectKey(int qtKey, const QString& text) {
    // Emit signal to QML
    emit keyEvent(text, qtKey);

    
    // Optionally inject to SDL
    SDL_Event sdlEvent{};
    sdlEvent.type = SDL_KEYDOWN;
    sdlEvent.key.state = SDL_PRESSED;
    sdlEvent.key.repeat = 0;
    
    // Convert text to SDL key symbol (very basic mapping)
    SDL_Keycode keycode = SDL_GetKeyFromName(text.toUtf8().constData());
    if (keycode == SDLK_UNKNOWN)
    keycode = static_cast<SDL_Keycode>(qtKey);  // fallback

    sdlEvent.key.keysym.sym = keycode;
    qDebug() << "Injecting key:" << text << "with SDL keycode:" << keycode;
    SDL_PushEvent(&sdlEvent);
}





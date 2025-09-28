### STANDARD CMAKE PROJECT SETUP
cmake_minimum_required(VERSION 3.16)
project(AppDeckRC
        VERSION 1.0
        DESCRIPTION "DeckRC - A Decky Plugin for Remote Control"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
### END OF STANDARD CMAKE PROJECT SETUP

### CUSTOM MESSAGES
message(STATUS "Project Name: ${PROJECT_NAME}")
message(STATUS "Config: ${CONFIG_NAME}")
message(STATUS "Source Dir: ${CMAKE_SOURCE_DIR}")
message(STATUS "Build Dir: ${CMAKE_BINARY_DIR}")
message(STATUS "Toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
### END OF CUSTOM MESSAGES


### LIBRARIES / PACKAGES
add_subdirectory("3rd/SDL-RC-Controller")
add_subdirectory("3rd/ExpressUSB-Controller")
add_subdirectory("3rd/OpenIPC-QML-Element")

find_package(SDL2 REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui Quick Qml Concurrent)
### END OF LIBRARIES / PACKAGES


### QT QUICK SETUP
qt_standard_project_setup()
qt_add_executable(${PROJECT_NAME}
    src/Core/main.cpp
)

qt_policy(SET QTP0004 NEW)
qt_policy(SET QTP0001 OLD)

qt_add_qml_module(${PROJECT_NAME}
    URI DeckRC
    VERSION 1.0
    QML_FILES
        src/UI/Main.qml
        src/UI/ChannelSettings.qml
        src/UI/RecordButton.qml
        src/UI/CaptureButton.qml
        src/UI/SettingsView.qml
        3rd/SDL-RC-Controller/qml/ChannelConfigurator_V2.qml
)

### LINKING
target_link_libraries(${PROJECT_NAME} PUBLIC 
    SDL_RC_Controller_Lib
)

target_link_libraries(${PROJECT_NAME} PUBLIC 
    ExpressUSB_Controller_Lib
)

target_link_libraries(${PROJECT_NAME} PUBLIC 
    OpenIPC_QML_Element_Lib
)

target_link_libraries(${PROJECT_NAME} PUBLIC 
    ${SDL2_LIBRARIES}
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
    Qt6::Quick
    Qt6::Qml
    Qt6::Concurrent
)
### END OF LINKING


### SOME QT INSTALL STUFF
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION  .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
### END OF SOME QT INSTALL STUFF

if(WIN32)
    find_program(QT_WINDEPLOYQT_EXECUTABLE windeployqt HINTS ${Qt6_DIR}/../../../bin)
    
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${QT_WINDEPLOYQT_EXECUTABLE} 
            --qmldir ${CMAKE_CURRENT_SOURCE_DIR}/src/UI  # For QML files
            # --debug                              # Use --release for release builds
            # --compiler-runtime                   # Include VC++ redistributables
            $<TARGET_FILE:${PROJECT_NAME}>
        COMMENT "Running windeployqt with custom options"
    )
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL2::SDL2>
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMENT "Copying SDL2 DLL"
    )
endif()
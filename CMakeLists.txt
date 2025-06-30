cmake_minimum_required(VERSION 3.16)
set(QT_QML_GENERATE_QMLLS_INI ON)
project(AppDeckRC
        VERSION 0.1
        DESCRIPTION "DeckRC - A Decky Plugin for Remote Control"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC on)

message(STATUS "Project Name: ${PROJECT_NAME}")
message(STATUS "Config: ${CONFIG_NAME}")
message(STATUS "Source Dir: ${CMAKE_SOURCE_DIR}")
message(STATUS "Build Dir: ${CMAKE_BINARY_DIR}")

# Find libraries
find_package(SDL2 REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS 
    Core
    Gui
    QML
    Quick
    QuickControls2
)
add_subdirectory(libs/Controller)

# QT setup 
message(STATUS "Using Qt version: ${Qt6Core_VERSION}")
qt_standard_project_setup()
qt_add_executable(${PROJECT_NAME}
    src/Core/main.cpp
)
qt_policy(SET QTP0004 NEW)
qt_policy(SET QTP0001 NEW)


qt_add_qml_module(${PROJECT_NAME}
    URI DeckRC
    VERSION 1.0
    QML_FILES
        src/UI/Screens/Main.qml
        src/UI/Screens/ChannelSettings.qml
    RESOURCES
        src/UI/Assets/Images/Background.png
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
# End of QT setup

# Libraries
target_include_directories(${PROJECT_NAME} PUBLIC libs/Controller/include)
target_link_directories(${PROJECT_NAME} PUBLIC libs/Controller)
target_link_libraries(${PROJECT_NAME} PUBLIC controller)

target_link_libraries(${PROJECT_NAME} PUBLIC SDL2::SDL2)

target_link_libraries(${PROJECT_NAME}
    PRIVATE Qt6::Core
    PRIVATE Qt6::Gui
    PRIVATE Qt6::Qml
    PRIVATE Qt6::Quick
    PRIVATE Qt6::QuickControls2
)

# QT specific stuff
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION  .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)



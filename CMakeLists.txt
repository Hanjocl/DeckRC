### STANDARD CMAKE PROJECT SETUP
cmake_minimum_required(VERSION 3.16)
project(AppDeckRC
        VERSION 0.1
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
message(STATUS "Install Lib Dir: ${CMAKE_INSTALL_LIBDIR}")
### END OF CUSTOM MESSAGES


### LIBRARIES / PACKAGES
find_package(SDL2 REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui Quick Qml)

add_subdirectory("lib/Test")
add_subdirectory("lib/Controller")
### END OF LIBRARIES / PACKAGES


### QT QUICK SETUP
qt_standard_project_setup()
qt_add_executable(${PROJECT_NAME}
    src/Core/main.cpp
    src/Core/inputControllerModel.h
    src/Core/inputControllerModel.cpp
)

qt_policy(SET QTP0004 NEW)
qt_policy(SET QTP0001 OLD)

qt_add_qml_module(${PROJECT_NAME}
    URI DeckRC
    VERSION 1.0
    QML_FILES
        src/UI/Main.qml
        src/UI/ChannelSettings.qml

    SOURCES
        src/Core/inputControllerModel.h
        src/Core/inputControllerModel.cpp
)

target_include_directories(AppDeckRC PRIVATE "src/Core/")
# if(WIN32)
#     set_target_properties(${PROJECT_NAME} PROPERTIES
#         MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
#         MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
#         MACOSX_BUNDLE TRUE
#         WIN32_EXECUTABLE TRUE
#     )
# endif()
### END OF  QT QUICK SETUP


### LINKING
target_link_libraries(${PROJECT_NAME} 
    PUBLIC TestLib
)
target_link_libraries(${PROJECT_NAME} 
    PUBLIC ControllerLib
)

target_link_libraries(${PROJECT_NAME} 
    PUBLIC SDL2::SDL2
)
target_link_libraries(${PROJECT_NAME}
    PUBLIC Qt6::Core
    PUBLIC Qt6::Widgets
    PUBLIC Qt6::Gui
    PUBLIC Qt6::Quick
    PUBLIC Qt6::Qml
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


add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "C:/Dev/SDL2/lib/x64/SDL2.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)
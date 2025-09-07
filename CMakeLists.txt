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

find_package(SDL2 REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui Quick Qml)

add_subdirectory("lib/Controller")
add_subdirectory("modules/OpenIPC-QML-Element")
add_subdirectory("modules/CRSF-USB-Host")

### END OF LIBRARIES / PACKAGES


### QT QUICK SETUP
qt_standard_project_setup()
qt_add_executable(${PROJECT_NAME}
    src/Core/main.cpp
    src/Core/inputControllerModel.h
    src/Core/inputControllerModel.cpp
    src/Core/inputTypes.h
    src/Core/ControllerModel/Worker.h
    src/Core/ControllerModel/Worker.cpp
    src/Core/ControllerModel/controllerModel.h
    src/Core/ControllerModel/controllerModel.cpp
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
        src/Core/ControllerModel/Worker.h
        src/Core/ControllerModel/Worker.cpp
        src/Core/ControllerModel/controllerModel.h
        src/Core/ControllerModel/controllerModel.cpp
    )

target_include_directories(AppDeckRC PUBLIC "src/Core/")
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
target_link_libraries(${PROJECT_NAME} PUBLIC 
    ControllerLib
)

target_link_libraries(${PROJECT_NAME} PUBLIC 
    CRSF_USB_Host
)

target_link_libraries(${PROJECT_NAME} PUBLIC 
    OpenIPC_QML_Element
)


target_link_libraries(${PROJECT_NAME} PUBLIC 
    ${SDL2_LIBRARIES}
)
# target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2 SDL2::SDL2main)

target_link_libraries(${PROJECT_NAME} PUBLIC
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
    Qt6::Quick
    Qt6::Qml
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
            --qmldir ${CMAKE_SOURCE_DIR}/src/UI  # For QML files
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

# add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different
#         "C:/Dev/SDL2/lib/x64/SDL2.dll"
#         $<TARGET_FILE_DIR:${PROJECT_NAME}>)
### STANDARD CMAKE PROJECT SETUP
cmake_minimum_required(VERSION 3.16)
project(AppDeckRC
        VERSION 1.0
        DESCRIPTION "DeckRC - A Decky Plugin for Remote Control"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

### CUSTOM MESSAGES
message(STATUS "Project Name: ${PROJECT_NAME}")
message(STATUS "Build Dir: ${CMAKE_BINARY_DIR}")
message(STATUS "Toolchain file: ${CMAKE_TOOLCHAIN_FILE}")

### LIBRARIES / PACKAGES
add_subdirectory("3rd/SDL-RC-Controller")
add_subdirectory("3rd/ExpressUSB-Controller")
add_subdirectory("3rd/OpenIPC-QML-Element")

find_package(SDL2 REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui Quick Qml Concurrent)

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
    ExpressUSB_Controller_Lib
    OpenIPC_QML_Element_Lib
    ${SDL2_LIBRARIES}
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
    Qt6::Quick
    Qt6::Qml
    Qt6::Concurrent
)

### INSTALL RULES
include(GNUInstallDirs)

# # Install QML files
# install(DIRECTORY src/UI DESTINATION ${CMAKE_INSTALL_BINDIR}/qml/DeckRC)
# install(DIRECTORY 3rd/SDL-RC-Controller/qml DESTINATION ${CMAKE_INSTALL_BINDIR}/qml/DeckRC)

### DEPLOYMENT FOLDER
set(DEPLOY_DIR "${CMAKE_BINARY_DIR}/deploy")
file(MAKE_DIRECTORY ${DEPLOY_DIR})

### COPY QML FILES
install(DIRECTORY src/UI DESTINATION ${DEPLOY_DIR}/qml/DeckRC)
install(DIRECTORY 3rd/SDL-RC-Controller/qml DESTINATION ${DEPLOY_DIR}/qml/DeckRC)

### WINDOWS: copy DLLs and run windeployqt
if(WIN32)
    # Copy all DLLs from binaryDir to deploy folder
    file(GLOB_RECURSE ALL_DLLS "${CMAKE_BINARY_DIR}/*.dll")
    foreach(DLL_FILE ${ALL_DLLS})
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${DLL_FILE}" "${DEPLOY_DIR}"
            COMMENT "Copying to deploy folder: ${DLL_FILE}"
        )
    endforeach()

    # Copy executable
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:${PROJECT_NAME}>
            ${DEPLOY_DIR}
        COMMENT "Copying executable to deploy folder"
    )

    # Run windeployqt into deploy folder
    find_program(QT_WINDEPLOYQT_EXECUTABLE windeployqt HINTS ${Qt6_DIR}/../../../bin)
    if(NOT QT_WINDEPLOYQT_EXECUTABLE)
        message(FATAL_ERROR "windeployqt not found!")
    endif()

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${QT_WINDEPLOYQT_EXECUTABLE}
            --qmldir ${CMAKE_CURRENT_SOURCE_DIR}/src/UI
            --dir ${DEPLOY_DIR}
            $<TARGET_FILE:${PROJECT_NAME}>
        COMMENT "Deploying Qt runtime with windeployqt"
    )

### LINUX: copy executable and shared libraries
elseif(UNIX AND NOT APPLE)
    # Generate a deployment script for the executable (Qt 6.5+)
    qt_generate_deploy_app_script(
        TARGET ${PROJECT_NAME}
        FILENAME_VARIABLE deploy_script
        NO_UNSUPPORTED_PLATFORM_ERROR
    )

    # Run the deployment script during install
    install(SCRIPT ${deploy_script})

    ## NEED MORE STUFF TO MAKE WORKING...
    # Qt version error with build on 6.9... and system using 6.8...
endif()

### CPACK PACKAGING
include(InstallRequiredSystemLibraries)
# Package name and version
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")

if(WIN32)
    set(CPACK_GENERATOR "ZIP")
    set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}-Windows-x86_64")
    elseif(UNIX AND NOT APPLE)
    set(CPACK_GENERATOR "TGZ")
    set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}-Linux-x86_64")
endif()


# Include only deploy folder
install(DIRECTORY ${DEPLOY_DIR}/
    DESTINATION .
    PATTERN "*.pdb" EXCLUDE     # optional: skip Windows debug symbols
)

# Optional: Short description
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
set(CPACK_PACKAGE_VENDOR "Untested Prototypes")
set(CPACK_PACKAGE_CONTACT "See github pageg")

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
include(CPack)
cmake_minimum_required(VERSION 3.16)
project(AppDeckRC
        VERSION 0.1
        DESCRIPTION "DeckRC - A Decky Plugin for Remote Control"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)

message(STATUS "Project Name: ${PROJECT_NAME}")
message(STATUS "Config: ${CONFIG_NAME}")
message(STATUS "Source Dir: ${CMAKE_SOURCE_DIR}")
message(STATUS "Build Dir: ${CMAKE_BINARY_DIR}")

find_package(SDL2 REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick)

qt_standard_project_setup()
qt_add_executable(${PROJECT_NAME}
    src/main.cpp
)

qt_add_qml_module(${PROJECT_NAME}
    URI DeckRC
    VERSION 1.0
    QML_FILES
        src/QML/Main.qml
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)
target_link_libraries(${PROJECT_NAME}
    PRIVATE Qt6::Core
    PRIVATE Qt6::Gui
    PRIVATE Qt6::Quick
)

#Clang formatting
#include(ClangFormat)
#add_clang_format_target(format-code ${PROJECT_SOURCE_DIR})  # add "-t format-code" to execute function

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION  .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)



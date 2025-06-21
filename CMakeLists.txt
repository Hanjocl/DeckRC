cmake_minimum_required(VERSION 3.10)
project(DeckRC
        VERSION 0.0.1
        DESCRIPTION "DeckRC - A Decky Plugin for Remote Control"
        LANGUAGES CXX)

set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
message(STATUS "Config: ${CONFIG_NAME}")
add_compile_definitions(CONFIG_NAME="${CONFIG_NAME}")


find_package(SDL2 REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick)

qt_standard_project_setup(REQUIRES 6.5)
qt_add_executable(DeckRC
    src/main.cpp
)
qt_add_qml_module(DeckRC
    URI ${CONFIG_NAME}
    QML_FILES
        src/QML/Main.qml
)

#add_executable(DeckRC src/main.cpp)

target_link_libraries(DeckRC PRIVATE SDL2::SDL2main SDL2::SDL2)
target_link_libraries(DeckRC PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Quick
)

#Clang formatting
include(ClangFormat)
#add_clang_format_target(format-code ${PROJECT_SOURCE_DIR})  # add "-t format-code" to execute function
cmake_minimum_required(VERSION 3.10)
project(DeckRC
        VERSION 0.0.1
        DESCRIPTION "DeckRC - A Decky Plugin for Remote Control"
        LANGUAGES CXX)

set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
message(STATUS "CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")
message(STATUS "PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
message(STATUS "VCPKG_PATH: ${CMAKE_TOOLCHAIN_FILE}")

find_package(SDL2 REQUIRED)
#find_package(Qt6 REQUIRED COMPONENTS Core)
# include_directories(DeckRC ${SDL2_INCLUDE_DIRS})

# if (NOT SDL2_FOUND)
#     message(FATAL_ERROR "SDL2 not found. Please install SDL2 development files.")
# endif()
add_executable(DeckRC src/main.cpp)
#target_include_directories(DeckRC PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(DeckRC PRIVATE SDL2::SDL2main SDL2::SDL2)
#target_link_libraries(DeckRC PRIVATE Qt6::Core)
#Clang formatting
include(ClangFormat)
#add_clang_format_target(format-code ${PROJECT_SOURCE_DIR})  # add "-t format-code" to execute function
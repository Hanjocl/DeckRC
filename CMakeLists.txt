cmake_minimum_required(VERSION 3.10)

set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

project(DeckRC
        VERSION 0.0.1
        DESCRIPTION "DeckRC - A Decky Plugin for Remote Control"
        LANGUAGES CXX)
        
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
message(STATUS "CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")
message(STATUS "PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")

find_package(SDL2 REQUIRED)

add_library(operations STATIC src/operations.cpp)
target_include_directories(operations PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(operations PUBLIC cxx_std_20)                           # Not required if using SET CMAKE_CXX_STANDARD, but good practice

add_library(dog STATIC src/dog.cpp)
target_include_directories(dog PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(dog PUBLIC cxx_std_20)                                  # Not required if using SET CMAKE_CXX_STANDARD, but good practice

add_executable(DeckRC src/main.cpp)
#target_include_directories(DeckRC PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(DeckRC PUBLIC operations)
target_link_libraries(DeckRC PUBLIC dog)

#Clang formatting
include(ClangFormat)
#add_clang_format_target(format-code ${PROJECT_SOURCE_DIR})  # add "-t format-code" to execute function
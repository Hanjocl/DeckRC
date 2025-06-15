cmake_minimum_required(VERSION 3.10)

set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

project(DeckRC
        VERSION 0.0.1
        DESCRIPTION "DeckRC - A Decky Plugin for Remote Control"
        LANGUAGES CXX)


add_library(operations STATIC src/operations.cpp)
target_include_directories(operations PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(operations PUBLIC cxx_std_20)                           # Not required if using SET CMAKE_CXX_STANDARD, but good practice



add_library(dog STATIC src/dog.cpp)
target_include_directories(dog PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(dog PUBLIC cxx_std_20)                                  # Not required if using SET CMAKE_CXX_STANDARD, but good practice

add_executable(DeckRC main.cpp)
#target_include_directories(DeckRC PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(DeckRC PUBLIC operations)
target_link_libraries(DeckRC PUBLIC dog)
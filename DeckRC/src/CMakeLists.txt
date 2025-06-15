cmake_minimum_required(VERSION 3.10)
project(DeckRC
        VERSION 0.0.1
        DESCRIPTION "DeckRC - A Decky Plugin for Remote Control"
        LANGUAGES CXX)

add_executable(DeckRC main.cpp)
target_compile_features(DeckRC PRIVATE cxx_std_20)
cmake_minimum_required(VERSION 3.16)

find_package(SDL2 REQUIRED)

add_library(controller 
src/controller.cpp 
src/behaviour.cpp
)

include_directories(${SDL2_INCLUDE_DIRS})
target_include_directories(controller PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(controller PUBLIC SDL2::SDL2)

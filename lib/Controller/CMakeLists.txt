cmake_minimum_required(VERSION 3.16)
project(ControllerLib)

# Needs SDL2 to work
find_package(SDL2 REQUIRED)

# Create library
add_library(${PROJECT_NAME} STATIC
    "src/inputController.cpp" 
    "src/behavior.cpp"
)
    
# Link SDL2
target_link_libraries(${PROJECT_NAME} PUBLIC SDL2::SDL2)

# Include the header directory
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")





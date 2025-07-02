cmake_minimum_required(VERSION 3.16)
project(TestLib)

include_directories("include")
# Create library
add_library(${PROJECT_NAME}
    "src/test.cpp"
)
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")


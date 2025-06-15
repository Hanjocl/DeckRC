# Define a function to create a custom target for running clang-format
function(add_clang_format_target TARGET_NAME SOURCE_DIR)
    find_program(CLANG-FORMAT_PATH .clang-format REQUIRED PATHS ${CMAKE_SOURCE_DIR})
    # Find all C++ source files in the specified directory
    file(GLOB_RECURSE FORMAT_SOURCES
        LIST_DIRECTORIES false
        "${SOURCE_DIR}/*.cpp"
        "${SOURCE_DIR}/*.h"
    )

    if(CLANG-FORMAT_PATH)
        message(STATUS "Found clang-format at: ${CLANG-FORMAT_PATH}")

        # Create a custom target to run clang-format
        add_custom_target(${TARGET_NAME}
            COMMAND ${CLANG-FORMAT_PATH} -i ${FORMAT_SOURCES}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}  # Change this if needed
            COMMENT "Running clang-format on ${SOURCE_DIR} sources"
        )
    else()
        message(FATAL_ERROR "clang-format not found. Please install it to use this feature.")
    endif()
    

    
endfunction()
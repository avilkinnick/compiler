cmake_minimum_required(VERSION 3.23)

project(compiler LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_executable(compiler)

target_sources(compiler
    PRIVATE
        src/HashTableString.c
        src/LinearAllocator.c
        src/main.c
        src/math_funcs.c

    PRIVATE
        FILE_SET HEADERS
        BASE_DIRS
            include
        FILES
            include/HashTableString.h
            include/LinearAllocator.h
            include/math_funcs.h
)

if(MSVC)
    target_compile_options(compiler
        PRIVATE
            /W3
    )
else()
    target_compile_options(compiler
        PRIVATE
            -Wall -Wextra -Wpedantic
    )
endif()

set_target_properties(compiler
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin
)

# Copyright (c) 2020 PotatoTech
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.13...3.17)

project(pong LANGUAGES C)

# Require out-of-source builds.
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "In-source builds are not allowed. Please make a build
subdirectory. Feel free to delete CMakeCache.txt and CMakeFiles.")
endif()

# If building on Windows, do not build as a console application.
if(WIN32)
    add_executable(${PROJECT_NAME} WIN32 "")
else()
    add_executable(${PROJECT_NAME} "")
endif()

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    C_STANDARD 99
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS OFF
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin"
)

foreach(output_config ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER "${output_config}" output_config)
    set_target_properties(${PROJECT_NAME}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_${output_config} "${PROJECT_BINARY_DIR}/bin"
    )
endforeach()

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/sdl2)

add_subdirectory(src)

# Set compiler warnings.
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
elseif(CMAKE_C_COMPILER_ID MATCHES "^GNU$|Clang$")
    target_compile_options(${PROJECT_NAME}
        PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Wshadow
            -Wconversion
            -Wsign-conversion
            -Werror=implicit-function-declaration
            -Werror=return-type
    )
endif()

# Enable interprocedural optimization for release builds if supported by
# the compiler.
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported)
if(ipo_supported)
    if(MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:/GL>)
        target_link_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:/LTCG>)
    else()
        string(TOUPPER "${CMAKE_BUILD_TYPE}" uppercase_build_type)
        if(uppercase_build_type STREQUAL "RELEASE")
            set_target_properties(${PROJECT_NAME} PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
        endif()
    endif()
endif()

get_target_property(runtime_output_dir ${PROJECT_NAME} RUNTIME_OUTPUT_DIRECTORY)
if(NOT runtime_output_dir)
    message(FATAL_ERROR "The runtime output directory for ${PROJECT_NAME} is not set.")
endif()

# Create the bin directory at build time.
add_custom_command(TARGET ${PROJECT_NAME}
    PRE_LINK
    COMMAND ${CMAKE_COMMAND} -E make_directory ${runtime_output_dir}
)

# Copy the res directory into bin at build time.
add_custom_command(TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/res ${runtime_output_dir}/res
    COMMENT "Copying resource files"
)

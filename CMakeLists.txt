#-------------------------------------------------------------------------------
# RESHAPE PROJECT CONFIGURATION
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.19)
project(Reshape)

#-------------------------------------------------------------------------------
# Utils / CMAKE Useful functions 
#-------------------------------------------------------------------------------

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")
include(CheckAndCloneLibrary)


#-------------------------------------------------------------------------------
# C++ Standard
#-------------------------------------------------------------------------------

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#-------------------------------------------------------------------------------
# Project Options
#-------------------------------------------------------------------------------

# Platform-specific support options
option(RESHAPE_ADD_SUPPORT_OPENGL "Enable OpenGL support" ON)
option(RESHAPE_ADD_SUPPORT_METAL "Enable Metal support for macOS/iOS" OFF)
option(RESHAPE_ADD_SUPPORT_VULKAN "Enable Vulkan support" OFF)
option(RESHAPE_ADD_SUPPORT_DIRECTX "Enable DirectX support" OFF)


option(RESHAPE_ADD_SUPPORT_PROFILING "Enable profiling support" OFF)

# Build type configuration
if(NOT RESHAPE_BUILD_TYPE)
    set(RESHAPE_BUILD_TYPE "Debug" CACHE STRING "Select the build type (Debug/Release/Distribution)" FORCE)
endif()
set_property(CACHE RESHAPE_BUILD_TYPE PROPERTY STRINGS Debug Release Distribution)

# Set CMAKE_BUILD_TYPE to match RESHAPE_BUILD_TYPE
set(CMAKE_BUILD_TYPE ${RESHAPE_BUILD_TYPE} CACHE STRING "" FORCE)

#-------------------------------------------------------------------------------
# Platform Selections and Defines
#-------------------------------------------------------------------------------

if(UNIX AND NOT APPLE AND NOT ANDROID AND NOT WEBGL)
    set(LINUX TRUE)
    add_compile_definitions(RESHAPE_PLATFORM_LINUX)
elseif(ANDROID)
    set(IS_MOBILE_TARGET TRUE)
    add_compile_definitions(RESHAPE_PLATFORM_ANDROID)
elseif(WEBGL)
    set(IS_MOBILE_TARGET TRUE)
    add_compile_definitions(RESHAPE_PLATFORM_WEBGL)
elseif(IOS)
    set(IS_MOBILE_TARGET TRUE)
    add_compile_definitions(RESHAPE_PLATFORM_IOS)
elseif(WIN32)
    add_compile_definitions(RESHAPE_PLATFORM_WINDOWS)
else()
    set(IS_HOST_PLATFORM TRUE)
    add_compile_definitions(RESHAPE_PLATFORM_UNKNOWN)
endif()

# Enable platform-specific options
if(RESHAPE_ADD_SUPPORT_METAL AND (IOS OR APPLE))
    add_compile_definitions(RESHAPE_SUPPORT_METAL)
endif()

if(RESHAPE_ADD_SUPPORT_VULKAN)
    add_compile_definitions(RESHAPE_SUPPORT_VULKAN)
endif()

if(RESHAPE_ADD_SUPPORT_OPENGL)
    add_compile_definitions(RESHAPE_SUPPORT_OPENGL)
endif()

if(RESHAPE_ADD_SUPPORT_DIRECTX AND WIN32)
    add_compile_definitions(RESHAPE_SUPPORT_DIRECTX)
endif()

#-------------------------------------------------------------------------------
# Global Configuration
#-------------------------------------------------------------------------------
# Base compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fexceptions")

# Build type specific configurations
if(RESHAPE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(RESHAPE_BUILD_DEBUG)
    set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
elseif(RESHAPE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(RESHAPE_BUILD_RELEASE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native")
    endif()
elseif(RESHAPE_BUILD_TYPE STREQUAL "Distribution")
    add_compile_definitions(RESHAPE_BUILD_DISTRIBUTION)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        set(CMAKE_CXX_FLAGS_DISTRIBUTION "-O3 -march=native -DNDEBUG")
    endif()
endif()

# Output configuration information
message(STATUS "Selected Build Type: ${RESHAPE_BUILD_TYPE}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Base Compiler Flags: ${CMAKE_CXX_FLAGS}")
message(STATUS "Build-Specific Flags: ${CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}}")

#-------------------------------------------------------------------------------
# Targets / Subdirectories
#-------------------------------------------------------------------------------

add_subdirectory(3rdparty)
add_subdirectory(Forge)
add_subdirectory(Reshape)


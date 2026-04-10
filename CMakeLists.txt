cmake_minimum_required(VERSION 3.20)
project(Soundlib LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(STATIC_SOUNDLIB "Build Soundlib as static library" OFF)
option(USE_SYSTEM_OPENAL "Use system-installed OpenAL implementation" ON)

include(FetchContent)

# -------------------------------
# OpenAL Soft
# -------------------------------
if (USE_SYSTEM_OPENAL)
    find_package(OpenAL REQUIRED)
else()
    set(ALSOFT_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(ALSOFT_TESTS OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(
        openal-soft
        GIT_REPOSITORY https://github.com/kcat/openal-soft.git
        GIT_TAG 1.24.3
    )
    FetchContent_MakeAvailable(openal-soft)

    # Create consistent name for target_link_libraries
    if (TARGET OpenAL AND NOT TARGET OpenAL::OpenAL)
        add_library(OpenAL::OpenAL ALIAS OpenAL)
    endif()
endif()

# -------------------------------
# Soundlib
# -------------------------------
set(SOUNDLIB_SOURCES
    src/stb_vorbis.c
    src/miniaudio.c
    src/soundlib.cpp
    src/sound.cpp
    src/sound_source.cpp
)

if (STATIC_SOUNDLIB)
    add_library(Soundlib STATIC ${SOUNDLIB_SOURCES})
else()
    add_library(Soundlib SHARED ${SOUNDLIB_SOURCES})
endif()

target_include_directories(Soundlib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
target_link_libraries(Soundlib PUBLIC OpenAL::OpenAL)

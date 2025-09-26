cmake_minimum_required(VERSION 3.13)
project(Soundlib LANGUAGES CXX C)

option(STATIC_SOUNDLIB "Build Soundlib as static library" OFF)
if (WIN32)
    option(USE_SYSTEM_OPENAL "Use system-installed OpenAL implementation" OFF)
else()
    option(USE_SYSTEM_OPENAL "Use system-installed OpenAL implementation" ON)
endif()

# -------------------------------
# OpenAL
# -------------------------------
if (USE_SYSTEM_OPENAL)
    find_package(OpenAL REQUIRED)
else()
    include(FetchContent)
    FetchContent_Declare(
        openal-soft
        GIT_REPOSITORY https://github.com/kcat/openal-soft.git
        GIT_TAG 1.24.3
    )
    FetchContent_MakeAvailable(openal-soft)

    # Create consistent name for target_link_libraries
    if (NOT TARGET OpenAL::OpenAL AND TARGET OpenAL)
        add_library(OpenAL::OpenAL ALIAS OpenAL)
    endif()
endif()

# -------------------------------
# Soundlib
# -------------------------------
if (STATIC_SOUNDLIB)
    add_library(Soundlib STATIC
        src/stb_vorbis.c
        src/miniaudio.c
        src/soundlib.cpp
        src/sound.cpp
        src/sound_source.cpp
    )
else()
    add_library(Soundlib SHARED
        src/stb_vorbis.c
        src/miniaudio.c
        src/soundlib.cpp
        src/sound.cpp
        src/sound_source.cpp
    )
endif()
target_include_directories(Soundlib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_link_libraries(Soundlib PRIVATE OpenAL::OpenAL)

# --------------------------
# Copy OpenAL library if built locally
# --------------------------
if (NOT USE_SYSTEM_OPENAL)
    add_custom_command(TARGET Soundlib POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:OpenAL>
            $<TARGET_FILE_DIR:Soundlib>
    )
endif()

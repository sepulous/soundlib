cmake_minimum_required(VERSION 3.13)
project(Soundlib LANGUAGES CXX C)

option(STATIC_SOUNDLIB "Build Soundlib as a static library")
if (WIN32)
    option(BUNDLE_OPENAL "Bundle OpenAL Soft instead of using system library" ON) # Better default for Windows
else()
    option(BUNDLE_OPENAL "Bundle OpenAL Soft instead of using system library" OFF)
endif()

# -------------------------------
# OpenAL
# -------------------------------
if (BUNDLE_OPENAL)
    include(FetchContent)
    FetchContent_Declare(
        openal-soft
        GIT_REPOSITORY https://github.com/kcat/openal-soft.git
        GIT_TAG 1.24.3
    )
    FetchContent_MakeAvailable(openal-soft)
    set(OPENAL_LIB OpenAL::OpenAL)
else()
    find_package(OpenAL REQUIRED)
    set(OPENAL_LIB OpenAL::OpenAL)
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

target_include_directories(Soundlib PUBLIC include)
target_link_libraries(Soundlib PRIVATE ${OPENAL_LIB})

# Copy OpenAL DLL next to library (if bundling on Windows)
if (WIN32 AND BUNDLE_OPENAL)
    add_custom_command(TARGET Soundlib POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:OpenAL::OpenAL>
            $<TARGET_FILE_DIR:Soundlib>
    )
endif()

# -------------------------------
# Install rules
# -------------------------------
install(TARGETS Soundlib
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
install(DIRECTORY include/ DESTINATION include)

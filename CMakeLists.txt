cmake_minimum_required(VERSION 3.14)
project(Gain
    VERSION 0.2.0
    DESCRIPTION "Lightweight cross-platform C++ OpenGL ES 2.0 engine"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# --- Sources ---------------------------------------------------------------
file(GLOB GAIN_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(Gain STATIC ${GAIN_SOURCES})
add_library(Gain::Gain ALIAS Gain)

target_include_directories(Gain PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include/gain>
)

# --- Dependencies ----------------------------------------------------------
# glm: required, system-provided. If your distro lacks a glm CMake config,
# install it (Linux: libglm-dev / glm; macOS: brew install glm).
find_package(glm CONFIG QUIET)
if(NOT glm_FOUND)
    # Fallback: fetch a pinned glm so the engine still builds out of the box.
    include(FetchContent)
    FetchContent_Declare(glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG        1.0.1
    )
    FetchContent_MakeAvailable(glm)
endif()

find_package(Threads REQUIRED)

target_link_libraries(Gain PUBLIC glm::glm Threads::Threads)

# CRITICAL: glm 1.0 changed default constructors from identity to zero
# (`glm::mat4()` is now a zero matrix; previously identity). gainlib's
# Base/Layer keep their `anim` matrices default-constructed and assume
# identity, so without this define every render multiplies by zero and
# nothing draws. PUBLIC so consumers see the same definition.
target_compile_definitions(Gain PUBLIC GLM_FORCE_CTOR_INIT)

# --- Platform GL backend ---------------------------------------------------
if(APPLE AND NOT IOS)
    # macOS desktop: GLBackend.h's TARGET_OS_IPHONE check picks <OpenGL/gl.h>.
    find_library(OPENGL_FRAMEWORK OpenGL REQUIRED)
    find_library(COCOA_FRAMEWORK   Cocoa  REQUIRED)
    target_link_libraries(Gain PUBLIC ${OPENGL_FRAMEWORK} ${COCOA_FRAMEWORK})
elseif(APPLE AND IOS)
    # iOS: GLBackend.h picks <OpenGLES/ES2/gl.h>; consumer's xcode toolchain
    # links the OpenGLES framework — we don't add it here so we stay
    # generator-agnostic.
elseif(ANDROID)
    target_link_libraries(Gain PUBLIC GLESv2 log android)
else()
    find_library(GLESV2_LIB NAMES GLESv2 REQUIRED)
    target_link_libraries(Gain PUBLIC ${GLESV2_LIB})
endif()

# --- Compile-time configuration -------------------------------------------
# (Threading switch is gone now that LOCK is std::mutex on every platform.
# Threads::Threads above still pulls in -lpthread on POSIX.)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    target_compile_options(Gain PRIVATE
        -Wall -Wextra
        -Wno-unused-parameter
        -Wno-unused-variable
    )
endif()

# --- Install (consumers can `find_package(Gain CONFIG)`) ------------------
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(TARGETS Gain
    EXPORT GainTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

file(GLOB GAIN_HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
install(FILES ${GAIN_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/gain)

install(EXPORT GainTargets
    FILE GainTargets.cmake
    NAMESPACE Gain::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Gain
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/GainConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

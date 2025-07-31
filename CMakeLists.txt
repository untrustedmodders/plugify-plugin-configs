cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

if(POLICY CMP0092)
    cmake_policy(SET CMP0092 NEW) # Don't add -W3 warning level by default.
endif()


file(READ "${CMAKE_CURRENT_SOURCE_DIR}/version.txt" VERSION_FILE_CONTENTS)
string(STRIP "${VERSION_FILE_CONTENTS}" VERSION_FILE_CONTENTS)
set(CONFIGS_VERSION "${VERSION_FILE_CONTENTS}" CACHE STRING "Set version name")
set(CONFIGS_PACKAGE "plugify-configs" CACHE STRING "Set package name")
string(REPLACE "v" "" CONFIGS_VERSION "${CONFIGS_VERSION}")
string(REGEX REPLACE "[.+-]" ";" CONFIGS_VERSION_LIST ${CONFIGS_VERSION})
list(GET CONFIGS_VERSION_LIST 0 CONFIGS_VERSION_MAJOR)
list(GET CONFIGS_VERSION_LIST 1 CONFIGS_VERSION_MINOR)
list(GET CONFIGS_VERSION_LIST 2 CONFIGS_VERSION_PATCH)

project(plugify-configs 
        VERSION "${CONFIGS_VERSION_MAJOR}.${CONFIGS_VERSION_MINOR}.${CONFIGS_VERSION_PATCH}"
        DESCRIPTION "Configs Plugin" 
        HOMEPAGE_URL "https://github.com/untrustedmodders" 
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE MATCHES "Debug|Devel|MinSizeRel|RelWithDebInfo|Release")
    message(STATUS "CMAKE_BUILD_TYPE not set, defaulting to Debug.")
    set(CMAKE_BUILD_TYPE Debug)
endif()

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

#
# Format
#
include(cmake/compat_format.cmake)

#
# Glaze
#
include(cmake/glaze.cmake)

#
# Plugin
#
file(GLOB_RECURSE PLUGIN_SOURCES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "src/*.cpp")

add_library(${PROJECT_NAME} SHARED ${PLUGIN_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/external/plugify/include")

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wextra -Wshadow -Wconversion -Wpedantic -Werror)
endif()

if(LINUX)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _GLIBCXX_USE_CXX11_ABI=0)
    target_link_libraries(${PROJECT_NAME} PRIVATE -static-libstdc++ -static-libgcc)
endif()

include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME} EXPORT_MACRO_NAME PLUGIFY_CONFIGS_API EXPORT_FILE_NAME "${CMAKE_BINARY_DIR}/exports/plugify_configs_export.h")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_BINARY_DIR}/exports")

if(APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE "-Wl,-exported_symbols_list,${CMAKE_CURRENT_SOURCE_DIR}/sym/exported_symbols.lds")
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME} PRIVATE "-Wl,--version-script,${CMAKE_CURRENT_SOURCE_DIR}/sym/version_script.lds")
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE 
        PLUGIFY_FORMAT_SUPPORT=$<BOOL:${COMPILER_SUPPORTS_FORMAT}>
        PLUGIFY_IS_DEBUG=$<STREQUAL:${CMAKE_BUILD_TYPE},Debug>)
if(NOT COMPILER_SUPPORTS_FORMAT)
    target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt-header-only)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE glaze::glaze)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/plugify-configs.pplugin.in
    ${CMAKE_CURRENT_BINARY_DIR}/plugify-configs.pplugin
)

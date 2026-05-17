cmake_minimum_required(VERSION 3.21)

# Полностью отключаем constexpr-проверки fmt на уровне всего компилятора
add_compile_options(-DFMT_ENFORCE_COMPILE_STRING_CHECK=0)
add_compile_options(-DFMT_USE_CONSTEXPR=0)
add_compile_options(-DFMT_NO_FMT_UTC_TIMESTAMP=1)

if(ANDROID)
    add_compile_options(-fno-constexpr-steps)
endif()

# Настройки стандарта
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
else()
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif()

set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(Template VERSION 1.0.0)

# Глобальные дефайны (дублируем для надежности)
add_compile_definitions(FMT_ENFORCE_COMPILE_STRING_CHECK=0)
add_compile_definitions(FMT_USE_CONSTEXPR=0)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_library(${PROJECT_NAME} SHARED ${SOURCES})

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode SDK.")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)
setup_geode_mod(${PROJECT_NAME})

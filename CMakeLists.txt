cmake_minimum_required(VERSION 3.21)

# Переключаем на стабильный стандарт C++20 (C++23 ломает старый fmt на Android)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
else()
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif()

set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(Template VERSION 1.0.0)

# ГЛОБАЛЬНЫЕ ФИКСЫ ДЛЯ FMT (Должны быть строго ДО загрузки Geode SDK)
add_compile_definitions(FMT_ENFORCE_COMPILE_STRING_CHECK=0)
add_compile_definitions(FMT_NO_FMT_UTC_TIMESTAMP=1)
add_compile_definitions(FMT_STATIC_THROWS=1)

# Сканируем все исходники в папке src
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

# Создаем бинарник мода
add_library(${PROJECT_NAME} SHARED ${SOURCES})

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode SDK.")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

# Подключаем поддиректорию Geode
add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

# Настраиваем зависимости Geode
setup_geode_mod(${PROJECT_NAME})

# Дополнительно защищаем наш собственный бинарник мода от строгих проверок строк
target_compile_definitions(${PROJECT_NAME} PRIVATE FMT_ENFORCE_COMPILE_STRING_CHECK=0)

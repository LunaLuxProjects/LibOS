cmake_minimum_required(VERSION 3.22.0)
project(libos CXX)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/CMake_Modules)

#options
option(USE_SANATIZER ON)
set(BUILD_OF_JNI OFF)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_COMPILER_NUMBER 0)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_COMPILER_NUMBER 1)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_COMPILER_NUMBER 2)
else()
    message(FATAL_ERROR "compiler not supported")
endif()

#platform independent code
set(LibOS_Source
        # other
        src/lib/FileIO.cpp
        # Interface
        src/lib/Interface/Cpp/GraphicsContext.cpp
        src/lib/Interface/Cpp/AudioManager.cpp
        src/lib/Interface/Cpp/Window.cpp
        src/lib/Interface/Cpp/Main.cpp)


if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(CMAKE_SYSTEM_NUMBER 0)
    message(INFO " - building for Linux....")
    include(${PROJECT_SOURCE_DIR}/CMake_Build_Platforms/Linux.cmake)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "WindowsStore")
    set(CMAKE_SYSTEM_NUMBER 1)
    message(INFO " -  building for WindowsStore....")
    include(${PROJECT_SOURCE_DIR}/CMake_Build_Platforms/WindowsStore.cmake)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(CMAKE_SYSTEM_NUMBER 2)
    message(INFO " - building for Windows....")
    include(${PROJECT_SOURCE_DIR}/CMake_Build_Platforms/Windows.cmake)
else()
    message(FATAL_ERROR "system not supported")
endif()
configure_file(src/tests/Cmake.h.in ../src/tests/Cmake.h @ONLY)

set(tests_sources src/tests/Main.cpp
    src/tests/FileIOTest.cpp
    src/tests/NetIOTest.cpp)
add_executable(LibOSTests ${tests_sources})
target_include_directories(LibOSTests PRIVATE ${PROJECT_SOURCE_DIR}/Includes)
#target_compile_options(LibOSTests PRIVATE -Wall -Wextra -Wpedantic -Werror -g -fsanitize=undefined,address -fstack-protector-all -fno-omit-frame-pointer)
#target_link_options(LibOSTests PRIVATE -fsanitize=undefined,address -fstack-protector-all -fno-omit-frame-pointer)
target_link_libraries(LibOSTests libos)
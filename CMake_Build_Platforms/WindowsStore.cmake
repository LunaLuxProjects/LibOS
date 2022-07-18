set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_CXX_EXTENSIONS ON)

configure_file(${PROJECT_SOURCE_DIR}/src/lib/Cmake.h.in ${PROJECT_SOURCE_DIR}/src/lib/Cmake.h @ONLY)

set(sources
    #common (move into interface)
    src/lib/FileIO.cpp
    # Interface
    src/lib/Interface/Cpp/Window.cpp
    src/lib/Interface/Cpp/Main.cpp

    # windows
    src/lib/Windows/FileIO/WinRTFileIO.cpp
    src/lib/Windows/Window/WinRTWindow.cpp

    # posix
    src/lib/Posix/NetIO.cpp)

add_library(libos SHARED ${sources})
target_include_directories(libos PRIVATE ${PROJECT_SOURCE_DIR}/Includes)

if(MSVC)
  target_compile_options(libos PRIVATE /W4 /WX)
else()
  target_compile_options(libos PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

set_target_properties(libos PROPERTIES VS_WINRT_COMPONENT TRUE VS_GLOBAL_ROOTNAMESPACE "libos" OUTPUT_NAME "libos")

if(${USE_SANATIZER})
    if(MSVC)
      target_compile_options(libos PRIVATE /fsanitize=address)
    else()
      target_compile_options(libos PRIVATE -fsanitize=address,alignment,bounds,null,unreachable,integer -fno-omit-frame-pointer)
    endif()
endif()

target_link_libraries(libos PRIVATE ${LINK_LIBS}) 
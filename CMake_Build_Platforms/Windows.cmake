set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_CXX_EXTENSIONS ON)

configure_file(${PROJECT_SOURCE_DIR}/src/lib/Cmake.h.in ${PROJECT_SOURCE_DIR}/src/lib/Cmake.h @ONLY)

set(sources 
    # other 
    src/lib/RefractileContext.cpp
    src/lib/FileIO.cpp
    # Interface
    src/lib/Interface/Cpp/AudioManager.cpp
    src/lib/Interface/Cpp/Window.cpp
    src/lib/Interface/Cpp/Main.cpp
    # windows
    src/lib/Windows/Audio/Win32AudioManager.cpp
    src/lib/Windows/FileIO/Win32FileIO.cpp
    src/lib/Windows/Window/Win32Window.cpp
    # posix
    src/lib/Posix/NetIO.cpp)

add_library(libos SHARED ${sources})

if(MSVC)
  target_compile_options(libos PRIVATE /W4 /WX)
else()
  target_compile_options(libos PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

if(${USE_SANATIZER})
    if(MSVC)
      target_compile_options(libos PRIVATE /fsanitize=address)
    else()
      target_compile_options(libos PRIVATE -fsanitize=address,alignment,bounds,null,unreachable,integer -fno-omit-frame-pointer)
    endif()
endif()
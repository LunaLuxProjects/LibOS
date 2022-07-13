set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_CXX_EXTENSIONS ON)

configure_file(${PROJECT_SOURCE_DIR}/src/lib/Cmake.h.in ${PROJECT_SOURCE_DIR}/src/lib/Cmake.h @ONLY)

find_package(jsoncpp CONFIG REQUIRED)
find_package(Wayland REQUIRED)
find_package(X11 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(unofficial-vulkan-memory-allocator CONFIG REQUIRED)
find_package(OpenAL CONFIG REQUIRED)
find_package(FreeALUT CONFIG REQUIRED)
include_directories(${X11_INCLUDE_DIR})
include_directories(${WAYLAND_INCLUDE_DIR})

if(${BUILD_OF_JNI})
  find_package(JNI REQUIRED)
  if(!${JNI_FOUND})
    message( FATAL_ERROR "jni is no available" )
  endif()
endif()

set(LibOS_Source src/lib/FileIO.cpp
        # Interface
        src/lib/Interface/Cpp/Window.cpp
        src/lib/Interface/Cpp/Main.cpp

        # posix
       src/lib/Posix/FileIO.cpp
       src/lib/Posix/NetIO.cpp

        # linux
        src/lib/Linux/Window/Wayland.cpp
       src/lib/Linux/Window/Xcb.cpp
        src/lib/Linux/Window/DirectScreen.cpp)

if(${BUILD_OF_JNI})
      set(LibOS_Source ${LibOS_Source}
      src/lib/Jni/FileIO.cpp
      src/lib/Jni/NetIO.cpp
      )
endif()

add_library(libos SHARED ${LibOS_Source})
target_compile_options(libos PRIVATE -Wall -Wextra -Wpedantic -Werror  -lwayland-client)

if(${USE_SANATIZER})
    target_compile_options(libos PRIVATE -fsanitize=address,alignment,bounds,null,unreachable,integer -fno-omit-frame-pointer)
endif()
set(LINK_LIBS "")
if(${BUILD_OF_JNI})
  set(LINK_LIBS ${JNI_LIBRARIES})
endif()

target_link_libraries(libos PRIVATE ${LINK_LIBS} ${WAYLAND_LIBRARIES} ${X11_xcb_LIB} ${X11_LIBRARIES} Vulkan::Vulkan OpenAL::OpenAL FreeALUT::alut unofficial::vulkan-memory-allocator::vulkan-memory-allocator JsonCpp::JsonCpp) 
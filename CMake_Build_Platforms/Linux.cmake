set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_CXX_EXTENSIONS ON)

configure_file(${PROJECT_SOURCE_DIR}/src/lib/Cmake.h.in ${PROJECT_SOURCE_DIR}/src/lib/Cmake.h @ONLY)

find_package(Wayland REQUIRED)
find_package(X11 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(ALSA REQUIRED)

if(${BUILD_OF_JNI})
  find_package(JNI REQUIRED)
  if(!${JNI_FOUND})
    message( FATAL_ERROR "jni is no available" )
  endif()
endif()

set(LibOS_Source ${LibOS_Source}
        # posix
        src/lib/Posix/FileIO.cpp
        src/lib/Posix/NetIO.cpp
        # linux
        src/lib/Linux/Graphics/Vulkan.cpp
        src/lib/Linux/Graphics/VkComponents/CVkMemoryManager.cpp
        src/lib/Linux/Graphics/VkComponents/CVkInstance.cpp
        src/lib/Linux/Graphics/VkComponents/CVkDevice.cpp
        src/lib/Linux/Audio/ALSAAudioManager.cpp
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
target_compile_options(libos PRIVATE -Wall -Wextra -Werror)
if(${CMAKE_COMPILER_NUMBER} EQUAL 1)
  target_compile_options(libos PRIVATE -lgcc)
endif()
#target_compile_options(libos PRIVATE -g -fsanitize=undefined,address -fstack-protector-all -fno-omit-frame-pointer)
#target_link_options(libos PRIVATE -fsanitize=undefined,address -fstack-protector-all -fno-omit-frame-pointer)
target_include_directories(libos PRIVATE ${X11_INCLUDE_DIR})
target_include_directories(libos PRIVATE ${WAYLAND_INCLUDE_DIR})
target_include_directories(libos PRIVATE ${Vulkan_INCLUDE_DIRS})
target_include_directories(libos PRIVATE ${ALSA_INCLUDE_DIRS})
target_include_directories(libos PRIVATE ${PROJECT_SOURCE_DIR}/Includes)
set(LINK_LIBS ${WAYLAND_LIBRARIES} ${X11_xcb_LIB} ${X11_LIBRARIES} ${Vulkan_LIBRARIES} ${ALSA_LIBRARIES})
if(${BUILD_OF_JNI})
  set(LINK_LIBS ${LINK_LIBS} ${JNI_LIBRARIES})
endif()

target_link_libraries(libos PRIVATE ${LINK_LIBS}) 
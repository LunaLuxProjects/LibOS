# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

#[=======================================================================[.rst:
FindWAYLAND
--------

Find wayland (WAYLAND)

Find the WAYLAND libraries (``wayland-client``)

IMPORTED Targets
^^^^^^^^^^^^^^^^

.. versionadded:: 3.12

This module defines :prop_tgt:`IMPORTED` target ``WAYLAND::WAYLAND``, if
WAYLAND has been found.

Result Variables
^^^^^^^^^^^^^^^^

This module defines the following variables:

``WAYLAND_FOUND``
  True if WAYLAND_INCLUDE_DIR & WAYLAND_LIBRARY are found

``WAYLAND_LIBRARIES``
  List of libraries when using WAYLAND.

``WAYLAND_INCLUDE_DIRS``
  Where to find the WAYLAND headers.

Cache variables
^^^^^^^^^^^^^^^

The following cache variables may also be set:

``WAYLAND_INCLUDE_DIR``
  the WAYLAND include directory

``WAYLAND_LIBRARY``
  the absolute path of the wayland-client library
#]=======================================================================]

find_path(WAYLAND_INCLUDE_DIR NAMES wayland-client.h
          DOC "The WAYLAND (wayland-client) include directory"
)

find_library(WAYLAND_LIBRARY NAMES wayland-client
          DOC "The WAYLAND (wayland-client) library"
)

if(WAYLAND_INCLUDE_DIR AND EXISTS "${WAYLAND_INCLUDE_DIR}/wayland-version.h")
  file(STRINGS "${WAYLAND_INCLUDE_DIR}/wayland-version.h" wayland_version_str REGEX "^#define[\t ]+WAYLAND_VERSION[\t ]+\".*\"")

  string(REGEX REPLACE "^.*WAYLAND_VERSION[\t ]+\"([^\"]*)\".*$" "\\1" WAYLAND_VERSION_STRING "${wayland_version_str}")
  unset(wayland_version_str)
endif()

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(WAYLAND
    REQUIRED_VARS
        WAYLAND_INCLUDE_DIR
        WAYLAND_LIBRARY 
    VERSION_VAR 
        WAYLAND_VERSION_STRING
)

if(WAYLAND_FOUND)
    set( WAYLAND_LIBRARIES ${WAYLAND_LIBRARY} )
    set( WAYLAND_INCLUDE_DIRS ${WAYLAND_INCLUDE_DIR} )
    if(NOT TARGET WAYLAND::WAYLAND)
        add_library(WAYLAND::WAYLAND UNKNOWN IMPORTED)
        set_target_properties(WAYLAND::WAYLAND PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${WAYLAND_INCLUDE_DIRS}")
        set_property(TARGET WAYLAND::WAYLAND APPEND PROPERTY IMPORTED_LOCATION "${WAYLAND_LIBRARY}")
    endif()
endif()
                                
mark_as_advanced(WAYLAND_INCLUDE_DIR WAYLAND_LIBRARY)
# This configuration file for finding libcheck is originally from the opensync
# project. The originally was downloaded from here:
# opensync.org/browser/branches/3rd-party-cmake-modules/modules/FindCheck.cmake
#
# Copyright (c) 2007 Daniel Gollub <dgollub@suse.de> Copyright (c) 2007 Bjoern
# Ricks  <b.ricks@fh-osnabrueck.de>
#
# Redistribution and use is allowed according to the terms of the New BSD
# license. For details see the accompanying COPYING-CMAKE-SCRIPTS file.

#[=======================================================================[.rst:
FindCheck
-------

Finds the Check library.

Imported Targets
^^^^^^^^^^^^^^^^

This module provides the following imported targets, if found:

``Check::Check
  The Check library

Result Variables
^^^^^^^^^^^^^^^^

This will define the following variables:

``Check_FOUND``
  True if the system has the Check library.
``Check_VERSION``
  The version of the Check library which was found.
``Check_INCLUDE_DIRS``
  Include directories needed to use Check.
``Check_LIBRARIES``
  Libraries needed to link to Check.

Cache Variables
^^^^^^^^^^^^^^^

The following cache variables may also be set:

``Check_INCLUDE_DIR``
  The directory containing ``check.h``.
``Check_LIBRARY``
  The path to the Check library.

#]=======================================================================]

find_package(PkgConfig)
pkg_search_module(PC_Check QUIET Check)

find_path(Check_INCLUDE_DIR
          NAMES check.h
          PATHS ${PC_Check_INCLUDE_DIRS} ${CHECK_INSTALL_DIR}
          PATH_SUFFIXES include)

find_library(Check_LIBRARY
             NAMES check
             PATHS ${PC_Check_LIBRARY_DIRS} ${CHECK_INSTALL_DIR})
find_library(Compat_LIBRARY
             NAMES compat
             PATHS ${PC_Check_LIBRARY_DIRS} ${CHECK_INSTALL_DIR})

set(Check_VERSION ${PC_Check_VERSION})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Check
                                  FOUND_VAR
                                  Check_FOUND
                                  REQUIRED_VARS
                                  Check_LIBRARY
                                  Check_INCLUDE_DIR
                                  VERSION_VAR
                                  Check_VERSION)

if(Check_FOUND)
  set(CHECK_LIBRARIES "${Check_LIBRARY}" "${Compat_LIBRARY}")
  set(CHECK_INCLUDE_DIRS "${Check_INCLUDE_DIR}")
endif()

# Provide imported target
if(Check_FOUND AND NOT TARGET Check::Check)
  add_library(Check::Check UNKNOWN IMPORTED)
  set_target_properties(Check::Check
                        PROPERTIES IMPORTED_LOCATION
                                   "${Check_LIBRARY}"
                                   INTERFACE_COMPILE_OPTIONS
                                   "${PC_Check_CFLAGS_OTHER}"
                                   INTERFACE_INCLUDE_DIRECTORIES
                                   "${Check_INCLUDE_DIR}")
endif()

mark_as_advanced(Check_INCLUDE_DIR Check_LIBRARY)

# compatibility variables
set(Check_VERSION_STRING ${Check_VERSION})

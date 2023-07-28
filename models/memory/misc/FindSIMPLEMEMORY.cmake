# This module looks for SimpleMemory module and determines where the header
# files and libraries are.

#=============================================================================
# Copyright 2014 GreenSocs
#
# KONRAD Frederic <fred.konrad@greensocs.com>
#
#=============================================================================

MESSAGE(STATUS "Searching for SimpleMemory model.")

# The HINTS option should only be used for values computed from the system.
SET(_SIMPLEMEMORY_HINTS
  ${SIMPLEMEMORY_PREFIX}/include
  $ENV{SIMPLEMEMORY_PREFIX}/include
  ${CMAKE_INSTALL_PREFIX}/include
  )
# Hard-coded guesses should still go in PATHS. This ensures that the user
# environment can always override hard guesses.
SET(_SIMPLEMEMORY_PATHS
  /usr/include
  )

FIND_PATH(SIMPLEMEMORY_INCLUDE_DIRS
  NAMES SimpleMemory/simpleMemory.h
  HINTS ${_SIMPLEMEMORY_HINTS}
  PATHS ${_SIMPLEMEMORY_PATHS}
)

if("${SIMPLEMEMORY_INCLUDE_DIRS}" MATCHES "SIMPLEMEMORY_INCLUDE_DIRS-NOTFOUND")
    SET(SIMPLEMEMORY_FOUND FALSE)
else("${SIMPLEMEMORY_INCLUDE_DIRS}" MATCHES "SIMPLEMEMORY_INCLUDE_DIRS-NOTFOUND")
    SET(SIMPLEMEMORY_FOUND TRUE)
    MESSAGE(STATUS "SimpleMemory include directory = ${SIMPLEMEMORY_INCLUDE_DIRS}")
endif("${SIMPLEMEMORY_INCLUDE_DIRS}" MATCHES "SIMPLEMEMORY_INCLUDE_DIRS-NOTFOUND")


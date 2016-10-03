cmake_minimum_required(VERSION 3.1)

project ( dp16 )

include(prj.cmake/dpInstall.cmake)

# Options
# -----------------------------------------------------------------------

# Determine the platform
# -----------------------------------------------------------------------
if(WIN32)
  set(PLATFORM_NAME "win")
elseif(UNIX)
  set(PLATFORM_NAME "unix")
else()
  message(fatal_error "Unsupported platform")
endif()

# Determine platform bits
# -----------------------------------------------------------------------
if(CMAKE_SIZEOF_VOID_P EQUAL 4)
  set(PLATFORM_BITNESS "32")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(PLATFORM_BITNESS "64")
else()
  message(fatal_error "Unsupported bitness")
endif()


# Platform is the pair of system and its bitness
# -----------------------------------------------------------------------
set(PLATFORM "${PLATFORM_NAME}${PLATFORM_BITNESS}")
message( "Platform is ${PLATFORM}")

if(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
  set(COMPILER_ID "icl")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(COMPILER_ID "gcc")
elseif(MSVC90)
  set(COMPILER_ID "vc2008")
elseif(MSVC10)
  set(COMPILER_ID "vc2010")
elseif(MSVC11)
  set(COMPILER_ID "vc2012")
elseif(MSVC12)
  set(COMPILER_ID "vc2013")
elseif(MSVC14)
  set(COMPILER_ID "vc2015")
else()
  set(COMPILER_ID "unknown")
endif()
message("Compiler id is ${COMPILER_ID}")

set(PLATFORM "${PLATFORM}.${COMPILER_ID}")
message("Build suffix is ${PLATFORM}")

# Configuration
# -----------------------------------------------------------------------
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "release")
endif()
if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
  set( CONFIGURATION "debug" )
ELSE()
  set( CONFIGURATION "release" )
endif()
MESSAGE( "Configuration is ${CONFIGURATION}" )

# C Runtime Environment.
# -----------------------------------------------------------------------
if(MSVC)
  set(CRT "vc")
elseif(CMAKE_COMPILER_IS_GNUCXX)
  set(CRT "gnu")
elseif(MAC_OS OR APPLE)
  set(CRT "gnu")
else()
  message(fatal_error "Unsupported CRT")
endif()


# Setting output directories
# -----------------------------------------------------------------------
# -----------------------------------------------------------------------

# Runtime output directory
# -----------------------------------------------------------------------
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY         "${CMAKE_SOURCE_DIR}/bin.${PLATFORM}.${CONFIGURATION}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/bin.${PLATFORM}.debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin.${PLATFORM}.release")
if( BUILD_SUFFIX)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY         "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}.${BUILD_SUFFIX}")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}.${BUILD_SUFFIX}")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}.${BUILD_SUFFIX}")
endif()

# Library output directory
# -----------------------------------------------------------------------
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY         "${CMAKE_SOURCE_DIR}/bin.${PLATFORM}.${CONFIGURATION}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/bin.${PLATFORM}.debug")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin.${PLATFORM}.release")
if( BUILD_SUFFIX)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY         "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}.${BUILD_SUFFIX}")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG}.${BUILD_SUFFIX}")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE}.${BUILD_SUFFIX}")
endif()

# Archive output directory
# -----------------------------------------------------------------------
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY         "${CMAKE_SOURCE_DIR}/lib/${PLATFORM}.${CRT}/${CONFIGURATION}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/lib/${PLATFORM}.${CRT}/debug")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/lib/${PLATFORM}.${CRT}/release")
if( BUILD_SUFFIX)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY         "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}.${BUILD_SUFFIX}")
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}.${BUILD_SUFFIX}")
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE}.${BUILD_SUFFIX}")
endif()

# Find OpenCV
# -----------------------------------------------------------------------
find_package(OpenCV 3.1 REQUIRED)

# Find OpenMP
# -----------------------------------------------------------------------
find_package(OpenMP REQUIRED)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")

# Include directories
# -----------------------------------------------------------------------
include_directories(
  ${OpenCV_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}/prj.core
)

# Setup special definitions
# -----------------------------------------------------------------------
# -----------------------------------------------------------------------
if(MSVC)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_SCL_SECURE_NO_WARNINGS)
  if(MSVC90)
    add_definitions(/wd4482)
  endif(MSVC90)
  set(CMAKE_C_FLAGS "/Zp1 /Za /W3 /nologo")
  if( ${CONFIGURATION} MATCHES "debug" )
    set( CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "/LDd /MDd" )
  ELSE()
    set( CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "/LD /MD" )
  endif()
  # TODO: check hack usage
  # Directory with libraries link to.
  # Here is a hack to set right link directory in MSVC IDE.
  if( MSVC_IDE )
    LINK_DIRECTORIES( "${CMAKE_SOURCE_DIR}/lib/${PLATFORM}.${CRT}/$(ConfigurationName)/" )
  ELSE()
    LINK_DIRECTORIES( "${CMAKE_SOURCE_DIR}/lib/${PLATFORM}.${CRT}/${CONFIGURATION}" )
  endif()
endif(MSVC)

if( CMAKE_COMPILER_IS_GNUCXX )
  set( WITH_PCH OFF )
  LINK_DIRECTORIES( ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )
  ADD_DEFINITIONS(-D_REENTRANT)
  set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wcast-align -fPIC -Wno-unused -Wno-missing-field-initializers -ldl" )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wcast-align -fPIC -Wno-unused -Wno-missing-field-initializers -ldl" )
  set( CMAKE_INSTALL_RPATH "$ORIGIN" )
  set( CMAKE_BUILD_WITH_INSTALL_RPATH TRUE )

  if( ${CONFIGURATION} MATCHES "debug" )
    set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g" )
  ELSE()
    set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O4" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O4" )
  endif()

  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )

#  message( "CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}" )
#  message( "CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}" )

endif()


# Add project collections to build tree.
# -----------------------------------------------------------------------
add_subdirectory(config)
add_subdirectory(prj.cmake)
add_subdirectory(prj.core)

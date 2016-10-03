project( dpcore )

set(dpcore_SOURCES
  src/common.cpp
  src/filesystem.cpp
  src/object.cpp
  src/geometry.cpp
)

set(dpcore_HEADERS
  common.h
  filesystem.h
  object.h
  geometry.h
)

add_library(dpcore STATIC
  ${dpcore_SOURCES}
  ${dpcore_HEADERS}
)

target_link_libraries( dpcore ${OpenCV_LIBS} )

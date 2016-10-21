project( image_db_loader )

set(image_db_loader_SOURCES
  src/interval_string_parser.cpp
  src/images_config_parser.cpp
  src/image_loader.cpp
)

set(image_db_loader_HEADERS
  interval_string_parser.h
  images_config_parser.h
  image_loader.h
)

add_library(image_db_loader STATIC
  ${image_db_loader_SOURCES}
  ${image_db_loader_HEADERS}
)

set(image_db_loader_LIBRARIES
  ${OpenCV_LIBS}
  dpcore
)

target_link_libraries( image_db_loader ${image_db_loader_LIBRARIES})

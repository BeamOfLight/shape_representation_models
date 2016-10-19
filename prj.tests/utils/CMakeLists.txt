project( image_db_loader_test )

set(image_db_loader_test_SOURCES
  src/image_db_loader_test.cpp
)

set(image_db_loader_test_LIBRARIES
  image_db_loader
  dpcore
)

add_executable(image_db_loader_test
  ${image_db_loader_test_SOURCES}
)

target_link_libraries( image_db_loader_test ${image_db_loader_test_LIBRARIES})


project( database_reader_config_test )

set(database_reader_config_test_SOURCES
  src/database_reader_config_test.cpp
)

set(database_reader_config_test_LIBRARIES
  ${OpenCV_LIBS}
  dpcore
)

add_executable(database_reader_config_test
  ${database_reader_config_test_SOURCES}
)

target_link_libraries( database_reader_config_test ${database_reader_config_test_LIBRARIES})


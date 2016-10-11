# Code from http://stackoverflow.com/a/18764531
# Neil Kirk http://ideone.com/2W99Tt

project( interval_string_parser_test )

set(interval_string_parser_test_SOURCES
  src/interval_string_parser_test.cpp
)

set(interval_string_parser_test_LIBRARIES
  interval_string_parser
)

add_executable(interval_string_parser_test
  ${interval_string_parser_test_SOURCES}
)

target_link_libraries( interval_string_parser_test ${interval_string_parser_test_LIBRARIES})


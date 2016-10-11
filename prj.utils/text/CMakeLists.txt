# Code from http://stackoverflow.com/a/18764531
# Neil Kirk http://ideone.com/2W99Tt

project( interval_string_parser )

set(interval_string_parser_SOURCES
  src/interval_string_parser.cpp
)

set(interval_string_parser_HEADERS
  interval_string_parser.h
)

add_library(interval_string_parser STATIC
  ${interval_string_parser_SOURCES}
  ${interval_string_parser_HEADERS}
)

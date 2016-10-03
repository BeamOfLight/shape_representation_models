project( shape_representation_models )

set(shape_representation_models_SOURCES
  src/main.cpp
  src/abstract_area_model.cpp
  src/abstract_other_model.cpp
  src/abstract_contour_model.cpp
  src/points.cpp
  src/shape_representation_experimental_stand.cpp
  src/freeman_chain_code_RLE.cpp
  src/difference_chain_code.cpp
  src/freeman_chain_code.cpp
  src/centered_points.cpp
  src/resampling_chain_code.cpp
  src/polygon_model.cpp
  src/grid.cpp
  src/grid_n.cpp
  src/quad_tree.cpp
  src/compressed_quad_tree.cpp
  src/fourier.cpp
  src/pyramid.cpp
  src/run_length_encoding.cpp
  src/deflate_model.cpp
)

set(shape_representation_models_HEADERS
  shape_representation_experimental_stand.h
  abstract_model.h
  abstract_area_model.h
  abstract_other_model.h
  abstract_contour_model.h
  freeman_chain_code_RLE.h
  difference_chain_code.h
  freeman_chain_code.h
  configuration.h
  points.h
  centered_points.h
  resampling_chain_code.h
  polygon_model.h
  grid.h
  grid_n.h
  quad_tree.h
  compressed_quad_tree.h
  fourier.h
  pyramid.h
  run_length_encoding.h
  deflate_model.h
)

set(shape_representation_models_LIBRARIES
  ${OpenCV_LIBS}
  dpcore
)


add_executable( shape_representation_models
  ${shape_representation_models_SOURCES}
  ${shape_representation_models_HEADERS}
)

target_link_libraries( shape_representation_models ${shape_representation_models_LIBRARIES})

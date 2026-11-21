add_library(aw9523b_core STATIC
    src/aw9523b.cpp
    src/result.cpp
    src/detail.hpp
)

target_compile_features(aw9523b_core
    PUBLIC cxx_std_20
)

set_target_properties(aw9523b_core PROPERTIES
    CXX_EXTENSIONS OFF
)

target_include_directories(aw9523b_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
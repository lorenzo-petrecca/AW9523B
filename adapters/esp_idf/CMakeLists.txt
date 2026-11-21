idf_component_register(
    SRCS
        "../../core/src/aw9523b.cpp"
        "../../core/src/result.cpp"
        "src/esp_idf_i2c.cpp"
        "src/esp_idf_platform.cpp"

    INCLUDE_DIRS
        "../../core/include"
        "include"

    PRIV_INCLUDE_DIRS
        "../../core/src"

    REQUIRES
        esp_driver_i2c

    PRIV_REQUIRES
        esp_driver_gpio
        esp_rom
)

target_compile_features(${COMPONENT_LIB}
    PUBLIC cxx_std_20
)

set_target_properties(${COMPONENT_LIB} PROPERTIES
    CXX_EXTENSIONS OFF
)

idf_component_register(SRCS "my_ble.c"
                    INCLUDE_DIRS "."
                    REQUIRES bt utils
)
target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format")
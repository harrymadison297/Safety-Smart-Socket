idf_component_register(SRCS "my_wifi.c" "./my_wifi.c"
                    INCLUDE_DIRS "."
                    REQUIRES esp_wifi lwip
)
target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format")
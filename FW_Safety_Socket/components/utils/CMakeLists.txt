idf_component_register(SRCS "my_utils.c" "./my_utils.c"
                    INCLUDE_DIRS "."
                    REQUIRES json nvs_flash
)

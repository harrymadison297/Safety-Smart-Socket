idf_component_register(SRCS "my_ade9153.c"
                    REQUIRES esp_driver_spi esp_driver_gpio
                    INCLUDE_DIRS "include")
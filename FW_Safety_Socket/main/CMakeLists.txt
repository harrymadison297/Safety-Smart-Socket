idf_component_register(SRCS "FW_Safety_Socket.c"
                    INCLUDE_DIRS "."
                    REQUIRES my_io nvs_flash bluetooth_le mqtt_protocol my_ade9153 utils wifi_mesh wifi_non_mesh)
idf_component_register(SRCS "mesh_netif.c" "./mesh_main.c" "./mesh_netif.c"
                    INCLUDE_DIRS "."
                    REQUIRES esp_wifi driver nvs_flash mqtt
)

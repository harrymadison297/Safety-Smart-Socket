#ifndef _MY_MESH_H_
#define _MY_MESH_H_
#include <stdint.h>

void esp_wifi_mesh_init(char *router_ssid, char *router_password, char *soft_ap_password, uint8_t mesh_id[6]);

#endif
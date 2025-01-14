#ifndef __MY_UTILS_H_
#define __MY_UTILS_H_

#include "esp_mac.h"
#include "esp_log.h"
#include "cJSON.h"
#include <string.h>
#include "nvs_flash.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

typedef struct
{
    char ble_pw[8];
    char router_ssid[100];
    char router_password[30];
    char client_id[30];

} router_and_client_id_infor_t;

typedef enum
{
    JOIN_NETWORK_ACCEPT = 0x01,
    RESET_SOFTAP_PASSWORD = 0x02,
    RESET_CHIP = 0x03,
    DIRECT_CONTROL = 0x04,
} cmd_e;

void get_mac_address(char *mac);
void mac_to_string(char *mac, char *mac_str);
router_and_client_id_infor_t parse_router_and_clientid_infor(char *data);
void save_router_and_clientid_infor(char *router_ssid, char *router_password, char *client_id);
void prepare_reset(void);
void delay_ms(int ms);
void create_join_request_json(const char *mac_str, const char *clientid, char *data);
void read_clientid_from_flash(char *client_id);
esp_err_t save_mesh_credentials(const char *mesh_id, const char *softap_pw);
esp_err_t save_flag_to_flash(bool flag);
bool read_flag_from_flash();
esp_err_t get_router_and_clientid_infor(router_and_client_id_infor_t *info);
esp_err_t get_mesh_credentials(char *mesh_id, char *softap_pw);
void hex_string_to_u8_array(char *hex_str, uint8_t *u8_array);
void create_voltage_current_json(const char *mac_str, float voltage, float current, bool state, char *json_buffer, size_t buffer_size);

#endif
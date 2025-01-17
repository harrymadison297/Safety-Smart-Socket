#ifndef __APP_MAIN_H_
#define __APP_MAIN_H_

#include "esp_err.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_system.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "my_mqtt.h"
#include "my_mesh.h"
#include "my_utils.h"
#include "my_ble.h"
#include "my_mqtt.h"
#include "my_wifi.h"
#include "my_ade9153.h"
#include "my_io.h"

/**
 * ESP32 Log Config
 */
#define TAG "MAIN"

/**
 * BLE Config
 */
#define BLE_PASSWORD "123456"

/**
 * MQTT Config
 */
#define MQTT_BROKER "mqtt://white-dev.aithings.vn:1883"
#define JOIN_REQ_FREQ 10000

/**
 * GPIO Define
 */
#define CTRL_ISO_PIN_OUT 26
#define STATE_LED_PIN_OUT 14
#define CONTROL_BUTTON_PIN 27

/**
 * ADE9153A Configuration
 */
#define ADE9153_PIN_RESET 22
#define RSHUNT 0.002 /* Ohm */
#define PGAGAIN 16
#define RBIG 1000000 /* Ohm */
#define RSMALL 1000	 /* Ohm */

#define xSPI_HOST VSPI_HOST
#define xSPI_CS -1
#define xSPI_CLK (1 * 1000 * 1000)

#define xSPI_INIT_DELAY 100 /* ms */
#define MESURE_PERIOD 500	/* ms */

#define HOLD_RESET_BUTTON_TIME_THRESHOLD 10000

/* Typedef Wifi Mesh Initial State */
typedef enum
{
	WIFI_MESH_NOT_INIT = false,
	WIFI_MESH_OK = true,
} wifi_mesh_init_e;

/* Typedef ADE9153A Data Interface */
typedef struct
{
	char *pNameDevice;
	char *version;
	uint8_t error;
	uint32_t sampleMSec;
	uint32_t sampleSecCalib;
	uint8_t voltSecCalib;
	uint8_t curtSecCalib;

	RMSRegs_t RMSValues;
	PowRegs_t PowValues;
	PQRegs_t PQValues;
	ACALRegs_t ACALValues;
} device_data_t;

/**
 * Global variable
 */
router_and_client_id_infor_t infor = {};
TaskHandle_t check_connect_wifi_task_handle;
TaskHandle_t ui_task_handle;
TaskHandle_t mesure_task_handle;
TaskHandle_t calib_task_handle;

/**
 * FreeRTOS Task declare
 */
void check_wifi_task(void *param);
void request_join_wifi_mesh_task(void *param);
void send_ui_to_cloud_task(void *parameter);
void ade9153a_mesurement_task(void *parameter);
void ade9153a_calib_task(void *parameter);
#endif
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "FW_Safety_Socket.h"

#define DEV_MODE 1
#define TEST_ADE9153 0

/**************************************************************
 *						GLOBAL VARIABLE
 **************************************************************/
/* Ade9153 data */
static RMSRegs_t RMSRegs;
static PowRegs_t PowRegs;
static PQRegs_t PQRegs;
//static ACALRegs_t ACALRegs;
static device_data_t devData;
/* Semaphore for BLE and Wifi Config */
static SemaphoreHandle_t sem;
/* ON/OFF Control state */
static bool control_state = false;

/**************************************************************
 *						TOOLS FUNCTIONS
 **************************************************************/
void esp_output_create(int pin)
{
	gpio_config_t pin_config = {
		1LL << pin,
		GPIO_MODE_OUTPUT,
		GPIO_PULLUP_DISABLE,
		GPIO_PULLDOWN_ENABLE,
		GPIO_INTR_DISABLE};
	gpio_config(&pin_config);
	gpio_set_direction(pin, GPIO_MODE_OUTPUT);
}

void esp_input_handler(void *arg)
{
    control_state = !control_state;
    printf("%d", control_state);
    gpio_set_level(CTRL_ISO_PIN_OUT, control_state);
    gpio_set_level(STATE_LED_PIN_OUT, control_state);
}

void esp_input_create()
{
    gpio_config_t pin_config = {
		1LL << CONTROL_BUTTON_PIN,
		GPIO_MODE_INPUT,
		GPIO_PULLUP_DISABLE,
		GPIO_PULLDOWN_DISABLE,
		GPIO_INTR_ANYEDGE
    };
    gpio_config(&pin_config);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(CONTROL_BUTTON_PIN, esp_input_handler, NULL);
}

void mqtt_recv_data_cb(char *topic, char *data)
{
    ESP_LOGI(TAG, "Get data from broker");
    ESP_LOGI(TAG, "%s", topic);
    ESP_LOGI(TAG, "%s", data);
    cJSON *json = cJSON_Parse(data);
    if (json == NULL)
    {
        ESP_LOGE(TAG, "PARSE MQTT DATA FAIL");
        return;
    }
    const int cmd = cJSON_GetObjectItem(json, "cmd")->valueint;
    switch (cmd)
    {
    case JOIN_NETWORK_ACCEPT:

        const char *mesh_id = cJSON_GetObjectItem(json, "mesh_id")->valuestring;
        const char *softap_pw = cJSON_GetObjectItem(json, "softap_pw")->valuestring;
        ESP_LOGI(TAG, "Accept join mesh id : %s and softap password %s", mesh_id, softap_pw);
        save_mesh_credentials(mesh_id, softap_pw);
        save_flag_to_flash(true);
        esp_restart();
        break;
    case RESET_SOFTAP_PASSWORD:
        break;
    case RESET_CHIP:
        nvs_flash_erase();
        esp_restart();
        break;

    default:
        break;
    }
}

void mqtt_sub_mac_topic_sc_cb(void)
{
    xTaskCreate((TaskFunction_t)request_join_wifi_mesh_task, "request join network", 4096, NULL, 1, NULL);
}

void wifi_connect_status_cb(bool status)
{
    switch (status)
    {
    case true:
        ESP_LOGI("WIFI STATUS CALLBACK", "OK");
        send_message_to_phone("OK", 2);
        mqtt_app_start(MQTT_BROKER);
        vTaskDelete(check_connect_wifi_task_handle);
        vSemaphoreDelete(sem);
        break;
    case false:
        ESP_LOGI("WIFI STATUS CALLBACK", "OK");
        send_message_to_phone("FAIL", 4);
        prepare_reset();
        esp_restart();
    default:
        break;
    }
}

void ble_recv_callback(char *data)
{
    ESP_LOGI(TAG, "Get data from phone %s", data);
    infor = parse_router_and_clientid_infor(data);
    if (strcmp(infor.ble_pw, BLE_PASSWORD) == 0)
    {
        xTaskCreate((TaskFunction_t)check_wifi_task, "Check wifi task", 8192, NULL, 1, (TaskHandle_t *)check_connect_wifi_task_handle);
        xSemaphoreGive(sem);
    }
    else
    {
        ESP_LOGE(TAG, "ble password wrong");
    }
}

/**************************************************************
 *						MAIN FUNCTION
 **************************************************************/
void app_main(void)
{
	/* Init flash memory */
	esp_err_t err;
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
	ESP_ERROR_CHECK(err);

	/* Read state flag from memory */
    bool value = false;
    value = read_flag_from_flash();
	ESP_LOGI(TAG, "Value = %d", value);

	sem = xSemaphoreCreateBinary();

    esp_output_create(CTRL_ISO_PIN_OUT);
    esp_output_create(STATE_LED_PIN_OUT);
    // esp_input_create();

    if (value == WIFI_MESH_NOT_INIT)
    {
        BLE_SERVER_INIT();
        ble_callback_register_callback(ble_recv_callback);
        register_wifi_status_callback(wifi_connect_status_cb);
        register_mqtt_recv_callback(mqtt_recv_data_cb);
        register_mqtt_sub_mac_topic_success(mqtt_sub_mac_topic_sc_cb);
    }
    
	if (value == WIFI_MESH_OK)
    {
        char mesh_id[30];
        char softap_pw[30];
        uint8_t mesh_id_u8[6];
        //router_and_client_id_infor_t info = {};
        register_mqtt_recv_callback(mqtt_recv_data_cb);
        get_router_and_clientid_infor(&infor);
        get_mesh_credentials(mesh_id, softap_pw);
        hex_string_to_u8_array(mesh_id, mesh_id_u8);
        esp_wifi_mesh_init(infor.router_ssid, infor.router_password, softap_pw, mesh_id_u8);
        mqtt_app_start(MQTT_BROKER);
        xTaskCreate((TaskFunction_t)send_ui_to_cloud_task, "send_data", 8192, (void *)infor.client_id, 1, (TaskHandle_t *)ui_task_handle);
		xTaskCreate((TaskFunction_t)ade9153a_mesurement_task, "ade9153a", 8192, NULL, 1, NULL);
    }
}

/**************************************************************
 *						FREERTOS TASK
 **************************************************************/
void ade9153a_mesurement_task(void *parameter)
{
	float vHeadRoom;
	bool checkSPI = false;

	/* Init Pin for startup ADE9153 */
	esp_output_create(ADE9153_PIN_RESET);
	esp_output_create(PIN_VSPI_CS);
	esp_output_create(PIN_VSPI_CLK);

	/* Choose ADE9153 SPI Communication */
	gpio_set_level(ADE9153_PIN_RESET, 1);
	gpio_set_level(PIN_VSPI_CS, 0);
	gpio_set_level(PIN_VSPI_CLK, 1);
	vTaskDelay(100 / portTICK_PERIOD_MS);
	gpio_set_level(ADE9153_PIN_RESET, 0);

	/* Initiation SPI to ADE9153 */
	do
	{
		checkSPI = init_spiADE9153(xSPI_HOST, xSPI_CS, xSPI_CLK);
		vTaskDelay(500 / portTICK_PERIOD_MS);
#if DEV_MODE
		if (!checkSPI)
			printf("SPI config fail\n");
		else
			printf("SPI config success\n");
#endif
	} while (!checkSPI);

#if TEST_ADE9153
	while (true)
	{
		spi_write32(REG_VNOM, 12);
		vTaskDelay(1000/portTICK_PERIOD_MS);
		ESP_LOGI(TAG, "REG_VNOM: %ld" ,spi_read32(REG_VNOM));
	}
#endif

	/* compute targetXCC */
	RMSRegs.targetAICC = calculate_target_aicc(RSHUNT, PGAGAIN);
	RMSRegs.targetAVCC = calculate_target_avcc(RBIG, RSMALL, &vHeadRoom);
	PowRegs.targetPowCC = calculate_target_powCC(RMSRegs.targetAICC, RMSRegs.targetAVCC);

    ADE9153_initCFG();

	while (true)
	{
		ADE9153_read_RMSRegs(&RMSRegs);
		ADE9153_read_PowRegs(&PowRegs);
		ADE9153_read_PQRegs(&PQRegs);
		devData.RMSValues = RMSRegs;
		devData.PowValues = PowRegs;
		devData.PQValues = PQRegs;
#if DEV_MODE
		printf("|- RMSRegs:\n");
		printf("|---- AIReg: %lx and AIValue: %f\n", RMSRegs.AIReg,
			   RMSRegs.AIValue);
		printf("|---- AVReg: %lx and AVValue: %f\n", RMSRegs.AVReg,
			   RMSRegs.AVValue);
		printf("|- PowRegs:\n");
		printf("|---- ActiveReg: %lx and activeValue: %f\n", PowRegs.activeReg,
			   PowRegs.activeValue);
		printf("|---- ReactiveReg: %lx and reactiveValue: %f\n",
			   PowRegs.reactiveReg, PowRegs.reactiveValue);
		printf("|---- ApparentReg: %lx and apparentValue: %f\n",
			   PowRegs.apparentReg, PowRegs.apparentValue);
		printf("|- PF compute: %f\n",
			   PowRegs.activeValue / PowRegs.apparentValue);
		printf("|- PQRegs:\n");
		printf("|---- pwFactorReg: %lx and pwFactorValue: %f\n",
			   PQRegs.pwFactorReg, PQRegs.pwFactorValue);
		printf("|---- periodReg: %lx and freqValue: %f\n", PQRegs.periodReg,
			   PQRegs.freqValue);
		printf("|---- angleAV_AIReg: %lx and angleAV_AIValue: %f\n",
			   PQRegs.angleAV_AIReg, PQRegs.angleAV_AIValue);
#endif
		vTaskDelay(MESURE_PERIOD / portTICK_PERIOD_MS);
	}
}

void check_wifi_task(void *param)
{
    while (1)
    {
        xSemaphoreTake(sem, portMAX_DELAY);
        save_router_and_clientid_infor(infor.router_ssid, infor.router_password, infor.client_id);
        wifi_connect(infor.router_ssid, infor.router_password);
    }
}

void request_join_wifi_mesh_task(void *param)
{
    char mac[6] = {0x00};
    char mac_str[15] = {0x00};
    char clientid[30];
    char data[100];
    get_mac_address(mac);
    mac_to_string(mac, mac_str);
    read_clientid_from_flash(clientid);
    ESP_LOGI(TAG, "%s and %s ", mac_str, clientid);
    create_join_request_json(mac_str, clientid, data);
    while (1)
    {
        mqtt_app_publish("/join_request", data, 1);
        delay_ms(JOIN_REQ_FREQ);
    }
}

void send_ui_to_cloud_task(void *parameter)
{
    char data[200];
    const char *client_id = (const char *)parameter;
    char mac[6] = {0x00};
    char mac_str[13] = {0x00};
    ESP_LOGI("TASK", "Client ID in task: %s", client_id);
    get_mac_address(mac);
    mac_to_string(mac, mac_str);
    char mqtt_topic[18];
    strcpy(mqtt_topic, "/mac/");
    strcat(mqtt_topic, mac_str);
    float i = 0.0;
    while (1)
    {
        i++;
        create_voltage_current_json(client_id, mac_str, devData.RMSValues.AVValue, devData.RMSValues.AIValue, data, sizeof(data));
        mqtt_app_publish(mqtt_topic, data, 1);
        delay_ms(MESURE_PERIOD);
    }
}


#include "my_utils.h"

#define UTILS_TAG "UTILS_TAG"
nvs_handle_t nvs_infor_handle;
void get_mac_address(char *mac)
{
    esp_read_mac((uint8_t *)mac, ESP_MAC_BT);
    ESP_LOGI(UTILS_TAG, "ESP32 bluetooth mac address : ");
    for (int i = 0; i < 6; i++)
    {
        ESP_LOGI(UTILS_TAG, "%02x", mac[i]);
    }
}

void mac_to_string(char *mac, char *mac_str)
{
    // Chuyển đổi địa chỉ MAC sang chuỗi định dạng XX:XX:XX:XX:XX:XX
    sprintf(mac_str, "%02x%02x%02x%02x%02x%02x",
            (unsigned char)mac[0], (unsigned char)mac[1], (unsigned char)mac[2],
            (unsigned char)mac[3], (unsigned char)mac[4], (unsigned char)mac[5]);

    ESP_LOGI(UTILS_TAG, "MAC Address as String: %s", mac_str);
}

router_and_client_id_infor_t parse_router_and_clientid_infor(char *data)
{
    router_and_client_id_infor_t infor = {};
    // Parse JSON string
    cJSON *json = cJSON_Parse(data);
    if (json == NULL)
    {
        ESP_LOGE(UTILS_TAG, "PARSE INFO FAIL");
        return infor;
    }

    // Extract fields
    const char *ble_pw = cJSON_GetObjectItem(json, "ble_pw")->valuestring;
    const char *router_ssid = cJSON_GetObjectItem(json, "router_ssid")->valuestring;
    const char *router_pw = cJSON_GetObjectItem(json, "router_pw")->valuestring;
    const char *clientid = cJSON_GetObjectItem(json, "clientid")->valuestring;

    strcpy(infor.ble_pw, ble_pw);
    strcpy(infor.router_ssid, router_ssid);
    strcpy(infor.router_password, router_pw);
    strcpy(infor.client_id, clientid);

    // Print extracted fields
    ESP_LOGI(UTILS_TAG, "BLE Password: %s\n", infor.ble_pw);
    ESP_LOGI(UTILS_TAG, "Router SSID: %s\n", infor.router_ssid);
    ESP_LOGI(UTILS_TAG, "Router Password: %s\n", infor.router_password);
    ESP_LOGI(UTILS_TAG, "Client ID: %s\n", infor.client_id);

    // Free JSON object
    cJSON_Delete(json);
    return infor;
}

void save_router_and_clientid_infor(char *router_ssid, char *router_password, char *client_id)
{
    esp_err_t err;
    err = nvs_open("storage", NVS_READWRITE, &nvs_infor_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(UTILS_TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    }
    err = nvs_set_str(nvs_infor_handle, "router_ssid", router_ssid);
    if (err != ESP_OK)
    {
        ESP_LOGE(UTILS_TAG, "Error saving router_ssid: %s\n", esp_err_to_name(err));
    }

    err = nvs_set_str(nvs_infor_handle, "router_password", router_password);
    if (err != ESP_OK)
    {
        ESP_LOGE(UTILS_TAG, "Error saving router_password: %s\n", esp_err_to_name(err));
    }

    err = nvs_set_str(nvs_infor_handle, "client_id", client_id);
    if (err != ESP_OK)
    {
        ESP_LOGE(UTILS_TAG, "Error saving client_id: %s\n", esp_err_to_name(err));
    }

    // Commit dữ liệu vào NVS
    err = nvs_commit(nvs_infor_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(UTILS_TAG, "Error committing changes: %s\n", esp_err_to_name(err));
    }

    // Đóng handle NVS
    nvs_close(nvs_infor_handle);

    ESP_LOGI(UTILS_TAG, "Router and client ID information saved successfully!\n");
}

// Hàm lấy thông tin từ NVS
esp_err_t get_router_and_clientid_infor(router_and_client_id_infor_t *info)
{
    esp_err_t err;
    nvs_handle_t nvs_infor_handle;

    // Mở NVS storage
    err = nvs_open("storage", NVS_READONLY, &nvs_infor_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(UTILS_TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return err;
    }

    // Đọc router_ssid
    size_t ssid_len = sizeof(info->router_ssid);
    err = nvs_get_str(nvs_infor_handle, "router_ssid", info->router_ssid, &ssid_len);
    if (err == ESP_ERR_NVS_NOT_FOUND)
    {
        ESP_LOGW(UTILS_TAG, "router_ssid not found, setting to empty string.");
        strcpy(info->router_ssid, "");
    }
    else if (err != ESP_OK)
    {
        ESP_LOGE(UTILS_TAG, "Error reading router_ssid: %s\n", esp_err_to_name(err));
        nvs_close(nvs_infor_handle);
        return err;
    }

    // Đọc router_password
    size_t password_len = sizeof(info->router_password);
    err = nvs_get_str(nvs_infor_handle, "router_password", info->router_password, &password_len);
    if (err == ESP_ERR_NVS_NOT_FOUND)
    {
        ESP_LOGW(UTILS_TAG, "router_password not found, setting to empty string.");
        strcpy(info->router_password, "");
    }
    else if (err != ESP_OK)
    {
        ESP_LOGE(UTILS_TAG, "Error reading router_password: %s\n", esp_err_to_name(err));
        nvs_close(nvs_infor_handle);
        return err;
    }

    // Đọc client_id
    size_t client_id_len = sizeof(info->client_id);
    err = nvs_get_str(nvs_infor_handle, "client_id", info->client_id, &client_id_len);
    if (err == ESP_ERR_NVS_NOT_FOUND)
    {
        ESP_LOGW(UTILS_TAG, "client_id not found, setting to empty string.");
        strcpy(info->client_id, "");
    }
    else if (err != ESP_OK)
    {
        ESP_LOGE(UTILS_TAG, "Error reading client_id: %s\n", esp_err_to_name(err));
        nvs_close(nvs_infor_handle);
        return err;
    }

    // Đọc ble_pw (nếu có)
    size_t ble_pw_len = sizeof(info->ble_pw);
    err = nvs_get_str(nvs_infor_handle, "ble_pw", info->ble_pw, &ble_pw_len);
    if (err == ESP_ERR_NVS_NOT_FOUND)
    {
        ESP_LOGW(UTILS_TAG, "ble_pw not found, setting to empty string.");
        strcpy(info->ble_pw, ""); // Gán chuỗi rỗng nếu không tồn tại
    }
    else if (err != ESP_OK)
    {
        ESP_LOGE(UTILS_TAG, "Error reading ble_pw: %s\n", esp_err_to_name(err));
        nvs_close(nvs_infor_handle);
        return err;
    }

    // Đóng NVS handle
    nvs_close(nvs_infor_handle);

    ESP_LOGI(UTILS_TAG, "Router and client ID information retrieved successfully!\n");
    return ESP_OK;
}

void prepare_reset(void)
{

    for (int i = 5; i > 0; i--)
    {
        ESP_LOGW(UTILS_TAG, "Prepare Reset node after %d second", i);
        delay_ms(1000);
    }
    nvs_flash_erase();
}

void delay_ms(int ms)
{
    vTaskDelay(ms / portTICK_PERIOD_MS);
}

void read_clientid_from_flash(char *client_id)
{
    esp_err_t err;

    // Mở NVS với không gian lưu trữ "storage"
    err = nvs_open("storage", NVS_READONLY, &nvs_infor_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(UTILS_TAG, "Error opening NVS handle: %s", esp_err_to_name(err));
        return;
    }

    // Lấy giá trị của client_id
    size_t required_size = 0; // Biến lưu kích thước chuỗi cần thiết
    err = nvs_get_str(nvs_infor_handle, "client_id", NULL, &required_size);
    if (err == ESP_OK && required_size > 0)
    {
        if (required_size <= 30)
        {
            err = nvs_get_str(nvs_infor_handle, "client_id", client_id, &required_size);
            if (err == ESP_OK)
            {
                ESP_LOGI(UTILS_TAG, "Client ID read successfully: %s", client_id);
            }
            else
            {
                ESP_LOGE(UTILS_TAG, "Error reading client_id: %s", esp_err_to_name(err));
            }
        }
        else
        {
            ESP_LOGE(UTILS_TAG, "Buffer too small to hold client_id");
        }
    }
    else if (err == ESP_ERR_NVS_NOT_FOUND)
    {
        ESP_LOGE(UTILS_TAG, "Client ID not found in NVS");
    }
    else
    {
        ESP_LOGE(UTILS_TAG, "Error checking client_id size: %s", esp_err_to_name(err));
    }

    // Đóng handle NVS
    nvs_close(nvs_infor_handle);
}

esp_err_t get_mesh_credentials(char *mesh_id, char *softap_pw)
{
    esp_err_t err;

    // Mở NVS
    err = nvs_open("storage", NVS_READONLY, &nvs_infor_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(UTILS_TAG, "Failed to open NVS");
        return err;
    }

    // Đọc mesh_id
    size_t mesh_id_len = 32; // Kích thước cố định của buffer mesh_id
    err = nvs_get_str(nvs_infor_handle, "mesh_id", mesh_id, &mesh_id_len);
    if (err == ESP_ERR_NVS_NOT_FOUND)
    {
        ESP_LOGW(UTILS_TAG, "mesh_id not found, setting to empty string.");
        strcpy(mesh_id, ""); // Gán chuỗi rỗng nếu không tồn tại
    }
    else if (err != ESP_OK)
    {
        ESP_LOGE(UTILS_TAG, "Failed to read mesh_id");
        nvs_close(nvs_infor_handle);
        return err;
    }

    // Đọc softap_pw
    size_t softap_pw_len = 30; // Kích thước cố định của buffer softap_pw
    err = nvs_get_str(nvs_infor_handle, "softap_pw", softap_pw, &softap_pw_len);
    if (err == ESP_ERR_NVS_NOT_FOUND)
    {
        ESP_LOGW(UTILS_TAG, "softap_pw not found, setting to empty string.");
        strcpy(softap_pw, ""); // Gán chuỗi rỗng nếu không tồn tại
    }
    else if (err != ESP_OK)
    {
        ESP_LOGE(UTILS_TAG, "Failed to read softap_pw");
        nvs_close(nvs_infor_handle);
        return err;
    }

    // Đóng NVS
    nvs_close(nvs_infor_handle);

    ESP_LOGI(UTILS_TAG, "Mesh credentials retrieved successfully");
    return ESP_OK;
}

void create_join_request_json(const char *mac_str, const char *clientid, char *data)
{
    // Tạo đối tượng JSON
    cJSON *root = cJSON_CreateObject();
    if (root == NULL)
    {
        ESP_LOGE("JSON", "Failed to create JSON object");
        return;
    }

    // Thêm mac_str vào JSON
    cJSON_AddStringToObject(root, "mac", mac_str);

    // Thêm clientid vào JSON
    cJSON_AddStringToObject(root, "clientid", clientid);

    // Chuyển đổi đối tượng JSON thành chuỗi
    char *json_string = cJSON_PrintUnformatted(root);
    if (json_string == NULL)
    {
        ESP_LOGE("JSON", "Failed to print JSON to string");
        cJSON_Delete(root);
        return;
    }
    ESP_LOGI(UTILS_TAG, "Check %s", json_string);
    // Sao chép chuỗi JSON vào `data`
    strcpy(data, json_string); // Giả định `data` đủ lớn

    // Dọn dẹp
    cJSON_Delete(root);
    free(json_string);
}

esp_err_t save_mesh_credentials(const char *mesh_id, const char *softap_pw)
{
    esp_err_t err;

    // Mở NVS
    err = nvs_open("storage", NVS_READWRITE, &nvs_infor_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(UTILS_TAG, "Failed to open NVS");
        return err;
    }

    // Lưu mesh_id
    err = nvs_set_str(nvs_infor_handle, "mesh_id", mesh_id);
    if (err != ESP_OK)
    {
        ESP_LOGE(UTILS_TAG, "Failed to write mesh_id");
        nvs_close(nvs_infor_handle);
        return err;
    }

    // Lưu softap_pw
    err = nvs_set_str(nvs_infor_handle, "softap_pw", softap_pw);
    if (err != ESP_OK)
    {
        ESP_LOGE(UTILS_TAG, "Failed to write softap_pw");
        nvs_close(nvs_infor_handle);
        return err;
    }

    // Ghi dữ liệu xuống flash
    err = nvs_commit(nvs_infor_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(UTILS_TAG, "Failed to commit changes");
        nvs_close(nvs_infor_handle);
        return err;
    }

    // Đóng NVS
    nvs_close(nvs_infor_handle);
    ESP_LOGI(UTILS_TAG, "Credentials saved to flash");
    return ESP_OK;
}

esp_err_t save_flag_to_flash(bool flag)
{
    esp_err_t err;

    err = nvs_open("storage", NVS_READWRITE, &nvs_infor_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(UTILS_TAG, "Failed to open NVS handle");
        return err;
    }

    // Lưu cờ vào flash (bool được lưu dưới dạng uint8_t)
    uint8_t value = flag ? 1 : 0; // Chuyển bool thành uint8_t
    err = nvs_set_u8(nvs_infor_handle, "flag", value);
    if (err != ESP_OK)
    {
        ESP_LOGE(UTILS_TAG, "Failed to set flag in NVS");
        nvs_close(nvs_infor_handle);
        return err;
    }

    // Ghi thay đổi vào flash
    err = nvs_commit(nvs_infor_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(UTILS_TAG, "Failed to commit changes to NVS");
        nvs_close(nvs_infor_handle);
        return err;
    }

    ESP_LOGI(UTILS_TAG, "FLAG value %d", flag);

    // Đóng NVS
    nvs_close(nvs_infor_handle);
    return ESP_OK;
}

bool read_flag_from_flash()
{
    esp_err_t err;

    // Mở NVS storage với namespace là "storage"
    err = nvs_open("storage", NVS_READWRITE, &nvs_infor_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(UTILS_TAG, "Failed to open NVS handle");
        return err;
    }
    uint8_t value = 0;
    err = nvs_get_u8(nvs_infor_handle, "flag", &value);
    ESP_LOGI(UTILS_TAG, "Get flag = %d", value);
    if (err == ESP_ERR_NVS_NOT_FOUND)
    {
        ESP_LOGW(UTILS_TAG, "Flag not found in flash, using default");
        nvs_close(nvs_infor_handle);
        return false;
    }
    else if (err != ESP_OK)
    {
        ESP_LOGE(UTILS_TAG, "Failed to get flag from NVS");
        nvs_close(nvs_infor_handle);
        return false;
    }

    ESP_LOGI(UTILS_TAG, "Flag read from flash: %d", value);

    // Đóng NVS
    nvs_close(nvs_infor_handle);
    return value;
}

void hex_string_to_u8_array(char *hex_str, uint8_t *u8_array)
{
    // Kiểm tra độ dài chuỗi hex (12 ký tự cho 6 byte)
    if (strlen(hex_str) != 12)
    {
        printf("Invalid hex string length. Must be 12 characters.\n");
        return;
    }

    // Duyệt qua từng cặp ký tự và chuyển đổi
    for (int i = 0; i < 6; i++)
    {
        char byte_str[3] = {hex_str[i * 2], hex_str[i * 2 + 1], '\0'}; // Tách 2 ký tự
        u8_array[i] = (uint8_t)strtol(byte_str, NULL, 16);             // Chuyển đổi thành uint8_t
    }
}

void create_voltage_current_json(const char *mac_str, float voltage, float current, bool state, char *json_buffer, size_t buffer_size)
{
    // Tạo đối tượng JSON gốc
    cJSON *root = cJSON_CreateObject();
    if (root == NULL)
    {
        ESP_LOGE("JSON", "Failed to create JSON object");
        return;
    }

    // Thêm MAC Address
    cJSON_AddStringToObject(root, "mac", mac_str);

    // Thêm State
    cJSON_AddBoolToObject(root, "state", state);

    // Thêm điện áp
    cJSON_AddNumberToObject(root, "voltage", voltage);

    // Thêm dòng điện
    cJSON_AddNumberToObject(root, "current", current);

    // Chuyển đổi đối tượng JSON sang chuỗi
    char *json_string = cJSON_PrintUnformatted(root);
    if (json_string == NULL)
    {
        ESP_LOGE("JSON", "Failed to convert JSON to string");
        cJSON_Delete(root);
        return;
    }

    // Sao chép chuỗi JSON vào buffer
    snprintf(json_buffer, buffer_size, "%s", json_string);

    // Giải phóng bộ nhớ JSON
    cJSON_Delete(root);
    free(json_string);
}

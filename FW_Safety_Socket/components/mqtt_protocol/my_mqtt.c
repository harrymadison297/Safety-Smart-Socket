/* Mesh IP Internal Networking Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <string.h>
#include "esp_log.h"
#include "esp_system.h"
#include "esp_netif.h"
#include "esp_tls.h"
#include "my_mqtt.h"
#include "esp_mac.h"

mqtt_data_recv_callback_t mqtt_data_recv_callback = NULL;
sub_mac_topic_success_callback_t sub_mac_topic_success_callback = NULL;
static const char *TAG = "mesh_mqtt";
static esp_mqtt_client_handle_t s_client = NULL;

static esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event)
{
    switch (event->event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        char mac[6] = {0x00};
        char mac_str[13] = {0x00};

        get_mac_address(mac);
        mac_to_string(mac, mac_str);
        ESP_LOGI(TAG, "Client subscribe to topic %s", mac_str);

        if (esp_mqtt_client_subscribe(s_client, mac_str, 1) < 0)
        {
            ESP_LOGE(TAG, "Subscribe fail, maybe lost connection");
            // Disconnect to retry the subscribe after auto-reconnect timeout
            esp_mqtt_client_disconnect(s_client);
        }

        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        break;

    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        bool value = read_flag_from_flash();
        if (value == 0)
        {
            sub_mac_topic_success_callback();
        }
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        char topic[100];
        char data[100];
        snprintf(topic, event->topic_len + 1, "%.*s", event->topic_len, event->topic);
        for (int i = 0; i < event->data_len; i++)
        {
            data[i] = event->data[i];
        }

        mqtt_data_recv_callback(topic, data);

        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
    return ESP_OK;
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%" PRId32 "", base, event_id);
    mqtt_event_handler_cb(event_data);
}

void mqtt_app_publish(char *topic, char *publish_string, int qos)
{
    if (s_client)
    {
        int msg_id = esp_mqtt_client_publish(s_client, topic, publish_string, 0, qos, 0);
        ESP_LOGI(TAG, "sent publish returned msg_id=%d", msg_id);
    }
}

void mqtt_app_subscribe(char *topic)
{
    if (esp_mqtt_client_subscribe(s_client, topic, 1) < 0)
    {
        // Disconnect to retry the subscribe after auto-reconnect timeout
        esp_mqtt_client_disconnect(s_client);
    }
}
void mqtt_app_start(char *host)
{
    ESP_LOGI(TAG, "Start MQTT");
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = host,
        .session.keepalive = 60,
        .network.reconnect_timeout_ms = 3000,
        .network.refresh_connection_after_ms = 5000,
        .network.timeout_ms = 3000,

    };
    s_client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(s_client, ESP_EVENT_ANY_ID, mqtt_event_handler, s_client);
    esp_mqtt_client_start(s_client);
}

void register_mqtt_recv_callback(void *cb)
{
    mqtt_data_recv_callback = cb;
}

void register_mqtt_sub_mac_topic_success(void *cb)
{
    sub_mac_topic_success_callback = cb;
}

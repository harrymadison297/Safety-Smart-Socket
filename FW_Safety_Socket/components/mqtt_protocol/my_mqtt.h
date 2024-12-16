#include "mqtt_client.h"
#include "my_utils.h"

typedef void (*mqtt_data_recv_callback_t)(char *topic, char *data);
typedef void (*sub_mac_topic_success_callback_t)(void);
void register_mqtt_recv_callback(void *cb);
void register_mqtt_sub_mac_topic_success(void *cb);
void mqtt_app_start(char *host);
void mqtt_app_subscribe(char *topic);
void mqtt_app_publish(char *topic, char *publish_string, int qos);

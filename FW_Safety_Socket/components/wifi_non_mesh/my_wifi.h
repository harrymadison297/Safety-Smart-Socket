#ifndef __MY_WIFI_H_
#define __MY_WIFI_H_

#define CONNECT_RETRY 3

typedef void (*wifi_connect_status_callback_t)(bool status);
void wifi_connect(char *router_ssid, char *router_password);
void register_wifi_status_callback(void *cb);
#endif

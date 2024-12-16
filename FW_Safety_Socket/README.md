| Supported Targets | ESP32 | ESP32-C3 | ESP32-C6 | ESP32-S2 | ESP32-S3 |
| ----------------- | ----- | -------- | -------- | -------- | -------- |

# Mesh IP Internal Networking 



## Functionality



### Hardware Required

This example can be executed on any platform board, the only required interface is WiFi and connection to internet.

### Configure the project


### Build and Flash

Build the project and flash it to multiple boards forming a mesh network, then run monitor tool to view serial output:

```
idf.py -p PORT flash monitor
```

(To exit the serial monitor, type ``Ctrl-]``.)

See the Getting Started Guide for full steps to configure and use ESP-IDF to build projects.

### Test send messge from phone to node (custom info)
```
{
  "ble_pw": "123456",
  "router_ssid": "Tuan T1",
  "router_pw": "tuan1234",
  "clientid": "MyClientID"
}
```
```
{
    "cmd": 1,
    "mesh_id": "665599227788",
    "softap_pw": "cuong_datn"
}
```

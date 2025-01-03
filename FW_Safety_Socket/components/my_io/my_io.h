#ifndef __MY_IO_H_
#define __MY_IO_H_
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

typedef void (*button_callback_t)(bool, uint64_t);
void esp_output_create(int pin_num);
void esp_input_create(int pin_num);
void button_callback_register(void *cb);
#endif
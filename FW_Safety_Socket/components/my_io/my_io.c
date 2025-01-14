
#include "my_io.h"
button_callback_t button_callback = NULL;
/* ON/OFF Control state */
static uint64_t start, end;
static uint64_t tick;
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

// bool status = false;
void esp_input_handler(void *arg)
{
    int gpio_num = (uint32_t)arg;
    // if (gpio_get_level(gpio_num) == 0)
    // {
        button_callback(0);
        start = xTaskGetTickCountFromISR();
    // }
    // else
    // {
        // end = xTaskGetTickCountFromISR();
        // tick = end - start;
        // button_callback(false, tick);
    // }
}

void esp_input_create(int pin_num)
{
    gpio_config_t pin_config = {
        1LL << pin_num,
        GPIO_MODE_INPUT,
        GPIO_PULLUP_ONLY,
        GPIO_PULLDOWN_DISABLE,
        GPIO_INTR_ANYEDGE};
    gpio_config(&pin_config);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(pin_num, esp_input_handler, NULL);
}

void button_callback_register(void *cb)
{
    button_callback = cb;
}

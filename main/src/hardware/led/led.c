
#include "led.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define LED_GPIO_PIN 2 

void led_init() {
    esp_rom_gpio_pad_select_gpio(LED_GPIO_PIN);
    gpio_set_direction(LED_GPIO_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_GPIO_PIN, 0); 
    ESP_LOGI("LED", "LED initialized");
}

void led_set(int state) {
    if (state) {
        gpio_set_level(LED_GPIO_PIN, 1);
    } else {
        gpio_set_level(LED_GPIO_PIN, 0);
    }
}
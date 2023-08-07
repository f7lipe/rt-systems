#include "led.h"

#ifdef __ESP_PLATFORM__ 
#include "driver/gpio.h"
#define LED_PIN 2
#else
#include <stdio.h>
#endif

void led_init(void) {
#ifdef __ESP_PLATFORM__
    gpio_pad_select_gpio(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_PIN, 0); 
#else
    printf("LED inicializado no computador.\n");
#endif
}

void led_on(void) {
#ifdef __ESP_PLATFORM__
    gpio_set_level(LED_PIN, 1);
#else
    printf("LED ligado no computador.\n");
#endif
}

void led_off(void) {
#ifdef __ESP_PLATFORM__
    gpio_set_level(LED_PIN, 0);
#else
    printf("LED desligado no computador.\n");
#endif
}
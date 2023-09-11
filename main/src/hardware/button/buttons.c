#include "buttons.h"
#include "driver/gpio.h"

#define BUTTON_GPIO_PIN 15 // Substitua pelo pino GPIO real que você está usando

void buttons_init() {
    esp_rom_gpio_pad_select_gpio(BUTTON_GPIO_PIN);
    gpio_set_direction(BUTTON_GPIO_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_GPIO_PIN, GPIO_PULLUP_ONLY);
}

bool button_is_pressed() {
    return gpio_get_level(BUTTON_GPIO_PIN) == 1; // Verifique se o nível está alto (pressionado)
}

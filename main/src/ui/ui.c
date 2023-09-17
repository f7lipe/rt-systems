#include "ui.h"
#include "src/hardware/led/led.h"
#include "src/hardware/display/display.h"
#include "src/hardware/button/buttons.h"
#include "esp_log.h"

static const char *TAG = "UI";

bool ui_init() {
    led_init();
    display_init();
    buttons_init();
    ESP_LOGI("UI", "UI initialized");
    return true;
}

void ui_update_display(float frequency) {
    ESP_LOGI(TAG, "Higher power frequency found: %.2f Hz", frequency);
}

void ui_update_led(int signal) {
    led_set(signal);
}

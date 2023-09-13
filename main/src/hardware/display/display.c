#include "display.h"
//#include "tm1637.h"
#include "esp_log.h"

#define DISPLAY_CLK_PIN 22 
#define DISPLAY_IO_PIN 23

//static tm1637_led_t *display;

void display_init() {
    //display = tm1637_init(DISPLAY_CLK_PIN, DISPLAY_IO_PIN);
    //tm1637_set_brightness(display, 7);  
    ESP_LOGI("DISPLAY", "Display initialized");
}

void display_show_frequency(int frequency) {
    // Atualize o display com a frequência fornecida
    // Por exemplo, mostre a frequência em formato decimal
    //char frequency_str[5];  // Supondo que a frequência máxima seja de 9999 Hz
    //snprintf(frequency_str, sizeof(frequency_str), "%04d", frequency);
    //tm1637_set_number(display, frequency_str);
}
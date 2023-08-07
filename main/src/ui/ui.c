#include "ui.h"
#include "../hardware/led/led.h"
#include "../hardware/display/display.h"

void ui_init(void) {
    display_init();
    led_init();
}

void ui_display_frequency(float frequency) {
    // Exibe a frequência no display de 7 segmentos
    // Exibe apenas a parte inteira da frequência

    int integer_part = (int)frequency;
    display_show_digit(0, integer_part / 1000); // Exibe o dígito mais significativo 
    display_show_digit(1, (integer_part / 100) % 10);
    display_show_digit(2, (integer_part / 10) % 10);
    display_show_digit(3, integer_part % 10);
}

void ui_show_signal_presence(bool presence) {
    if (presence) {
        led_on();
    } else {
        led_off(); 
    }
}
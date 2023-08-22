#include "ui.h"
#include "src/hardware/led/led.h"
#include "src/hardware/display/display.h"

void ui_init() {
    // Inicializar o display e o LED aqui
    led_init();
    display_init();
}

void ui_update_display(float frequency) {
    // Atualizar o display com a frequência
}

void ui_update_led(int signal) {
    led_set(signal);
}

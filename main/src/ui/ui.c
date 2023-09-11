#include "ui.h"
#include "src/hardware/led/led.h"
#include "src/hardware/display/display.h"
#include "src/hardware/button/buttons.h"

void ui_init() {
    // Inicializar o display e o LED e o botão aqui
    led_init();
    display_init();
    buttons_init();
}

void ui_update_display(float frequency) {
    // Atualizar o display com a frequência
}

void ui_update_led(int signal) {
    led_set(signal);
}

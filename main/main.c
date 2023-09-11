#include "src/ui/ui.h"
#include "src/hardware/microphone/mic.h"
#include "src/dsp/signal.h"
#include "src/hardware/button/buttons.h"
#include "esp_log.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define N_WINDOW_ELEMENTS 1024
#define POTENCY_WINDOW_SIZE 32
#define MUSIC_SIZE 1024
#define FRAME_RATE 16000
#define NOISE_THRESHOLD 2

int16_t window[N_WINDOW_ELEMENTS];
int16_t windowModified[N_WINDOW_ELEMENTS];
int16_t music[MUSIC_SIZE];
int potencies[POTENCY_WINDOW_SIZE];
int potencyActualIndex = 0;
int newPotency = 0;
int signalVelocity = 1;
int musicIndex = 0;

void update_button_state() {
    if (button_is_pressed()) {
        // Botão está pressionado, atualize o estado conforme necessário
        // Exemplo: Ciclo entre 1x, 2x, 3x, 4x e depois volte para 1x
        signalVelocity = (signalVelocity % 4) + 1;
        ESP_LOGI("BUTTON", "Button pressed, velocity is now %dx", signalVelocity);
    }
}


void app_main(void) {

    ui_init();
    mic_init();

    int16_t buffer[N_WINDOW_ELEMENTS];

    while (1) {
        mic_read_data(buffer, N_WINDOW_ELEMENTS);

        update_button_state();
        signal_modify_velocity(buffer, N_WINDOW_ELEMENTS, signalVelocity);

        bool presence = signal_detect_presence(buffer);
        float higherPowerFrequency = signal_find_higher_power_frequency(buffer);

        // Atualize a interface do usuário
        ui_update_display(higherPowerFrequency);
        ui_update_led(presence);
    }
    free(buffer);
}

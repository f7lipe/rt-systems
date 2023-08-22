#include "src/ui/ui.h"
#include "src/hardware/microphone/mic.h"
#include "src/dsp/signal.h"
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

void app_main(void) {

    ui_init();
    mic_init();

    int16_t buffer[N_WINDOW_ELEMENTS];

    while (1) {
        mic_read_data(buffer, N_WINDOW_ELEMENTS);

        // Copie o buffer para a janela (window) e realize as operações de processamento de sinal
        // com as funções definidas em signal.c

        bool presence = signal_detect_presence(buffer);
        float higherPowerFrequency = signal_find_higher_power_frequency(buffer);

        // Atualize a interface do usuário
        ui_update_display(higherPowerFrequency);
        ui_update_led(presence);
    }
    free(buffer);
}

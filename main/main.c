#include "esp_log.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "src/hardware/bluetooth/bluetooth.h"
#include "src/hardware/microphone/mic.h"
#include "src/hardware/button/buttons.h"
#include "src/utils/audio_utils.h"
#include "src/dsp/signal.h"
#include "src/ui/ui.h"
#include "esp_timer.h"

#define N_WINDOW_ELEMENTS 1024
int signalVelocity = 1;
int16_t buffer[N_WINDOW_ELEMENTS];

bool presence;
float higherPowerFrequency;

#define PERIOD_MICROPHONE_READ 64
#define PERIOD_VELOCITY_MODIFY 128
#define PERIOD_SIGNAL_PRESENCE 128
#define PERIOD_FIND_FREQUENCY 64
#define PERIOD_UPDATE_DISPLAY 256
#define PERIOD_UPDATE_LED 512


// rastrear o tempo de execução das tarefas
uint64_t lastMicrophoneReadTime = 0;
uint64_t lastVelocityModifyTime = 0;
uint64_t lastSignalPresenceTime = 0;
uint64_t lastFindFrequencyTime = 0;
uint64_t lastUpdateDisplayTime = 0;
uint64_t lastUpdateLedTime = 0;

void update_button_state()
{
    if (button_is_pressed())
    {
        //signalVelocity = (signalVelocity % 4) + 1;
        //ESP_LOGI("BUTTON", "Button pressed, velocity is now %dx", signalVelocity);
    }
}

void app_main(void)
{
    mic_init();
    ui_init();
    bluetooth_init();

    while (1)
    {
        uint64_t currentTime = esp_timer_get_time();

        if (bluetooth_is_connected())
        {
            // 64 ms
            if (currentTime - lastMicrophoneReadTime >= (PERIOD_MICROPHONE_READ / 1000))
            {
                mic_read_data(buffer, N_WINDOW_ELEMENTS);
                lastMicrophoneReadTime = currentTime;
            }

            // 128 ms
            if (currentTime - lastVelocityModifyTime >= (PERIOD_VELOCITY_MODIFY / 1000))
            {
                //signal_modify_velocity(buffer, N_WINDOW_ELEMENTS, signalVelocity);
                lastVelocityModifyTime = currentTime;
            }

            // 128 ms
            if (currentTime - lastSignalPresenceTime >= (PERIOD_SIGNAL_PRESENCE / 1000))
            {
                presence = signal_detect_presence(buffer);
                lastSignalPresenceTime = currentTime;
            }

            // 64 ms
            if (currentTime - lastFindFrequencyTime >= (PERIOD_FIND_FREQUENCY / 1000))
            {
                higherPowerFrequency = signal_find_higher_power_frequency(buffer);
                lastFindFrequencyTime = currentTime;
            }

            // 256 ms
            if (currentTime - lastUpdateDisplayTime >= (PERIOD_UPDATE_DISPLAY / 1000))
            {
                ui_update_display(higherPowerFrequency);
                lastUpdateDisplayTime = currentTime;
            }

            // 512 ms
            if (currentTime - lastUpdateLedTime >= (PERIOD_UPDATE_LED / 1000))
            {
                ui_update_led(presence);
                lastUpdateLedTime = currentTime;
            }
        }

        // Verificação de botão (fora da condição Bluetooth)
        update_button_state();
    }
}
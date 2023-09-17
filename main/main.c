#include "esp_log.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "src/hardware/bluetooth/bluetooth.h"
#include "src/hardware/microphone/mic.h"
#include "src/hardware/button/buttons.h"
#include "src/utils/audio_utils.h"
#include "src/dsp/signal.h"
#include "src/ui/ui.h"

#define N_WINDOW_ELEMENTS 1024
int signalVelocity = 1;
int16_t buffer[N_WINDOW_ELEMENTS];

void update_button_state()
{
    if (button_is_pressed())
    {
        signalVelocity = (signalVelocity % 4) + 1;
        ESP_LOGI("BUTTON", "Button pressed, velocity is now %dx", signalVelocity);
    }
}

void app_main(void)
{

    if ( mic_init() && ui_init() && bluetooth_init())
    {
        while (1)
        {
            if (bluetooth_is_connected())
            {
                mic_read_data(buffer, N_WINDOW_ELEMENTS);

                update_button_state();

                for (int i = 0; i < N_WINDOW_ELEMENTS; i++)
                {
                     audio_data.real_data[i] = buffer[i];
                }

                signal_modify_velocity(buffer, N_WINDOW_ELEMENTS, signalVelocity);
                bool presence = signal_detect_presence(buffer);
                float higherPowerFrequency = signal_find_higher_power_frequency(buffer);

                ui_update_display(higherPowerFrequency);
                ui_update_led(presence);
            }
        }
        free(buffer);
        bluetooth_cleanup();
    }
}

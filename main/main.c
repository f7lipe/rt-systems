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
float mix_level = 0.5; // Defina o valor apropriado de mix_level
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

    if (/*audio_utils_init() && */ mic_init() && ui_init() && bluetooth_init())
    {

        /*
        // Carregue o arquivo WAV da música
        int16_t* music_data = NULL;
        size_t music_size = 0;
        audio_utils_load_wav("/spiffs/zelda.wav", &music_data, &music_size);

        if (music_data == NULL || music_size == 0) {
            ESP_LOGE("MUSIC", "Failed to load music.");
            return;
        }
        */
        while (1)
        {
            if (bluetooth_is_connected())
            {
                mic_read_data(buffer, N_WINDOW_ELEMENTS);

                //update_button_state();

                for (int i = 0; i < N_WINDOW_ELEMENTS; i++)
                {
                     audio_data.real_data[i] = buffer[i];
                }

                bool presence = signal_detect_presence(buffer);
                float higherPowerFrequency = signal_find_higher_power_frequency(buffer);

                // Atualize a interface do usuário
                ui_update_display(higherPowerFrequency);
                ui_update_led(presence);
            }
        }

        // Libere a memória alocada para o áudio da música
        // free(music_data);
        free(buffer);
        bluetooth_cleanup();
    }
}

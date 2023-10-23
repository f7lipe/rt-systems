#include "mic.h"
#include <driver/i2s.h>
#include "esp_log.h"
#include "audio_utils.h"

#define WS_IO_PORT 22
#define SD_PORT 25
#define SCK_PORT 23
#define I2S_NUM I2S_NUM_0        // Número da interface I2S a ser usada
#define FRAME_RATE 44100          // Taxa de amostragem (44,1kHz)
#define N_WINDOW_ELEMENTS 1024    // Tamanho da janela de amostragem

Queue queueOut;

bool mic_init() {
i2s_pin_config_t pin_config = {
    .bck_io_num = SCK_PORT,
    .ws_io_num = WS_IO_PORT,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = SD_PORT};
i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = FRAME_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S_MSB,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = N_WINDOW_ELEMENTS,
    .use_apll = false};

    esp_err_t err = i2s_driver_install(I2S_NUM, &i2s_config, 0, NULL);
    esp_err_t err2 = i2s_set_pin(I2S_NUM, &pin_config);
    if (err != ESP_OK || err2 != ESP_OK) {
        ESP_LOGE("MIC", "Failed to install I2S driver (%s)", esp_err_to_name(err));
        return false;
    }
    initialize(&queueOut);

    ESP_LOGI("MIC", "Microphone initialized");
    return true;
}

int vector_index = 0;
int16_t microfoneData[N_WINDOW_ELEMENTS];
void mic_read_data(int16_t* buffer, int buffer_samples) {

    size_t bytes_read;
    // cout << "Iniciando acesso ao microfone." << endl;
    i2s_read(I2S_NUM_0, microfoneData, N_WINDOW_ELEMENTS, &bytes_read, 10);
    int buffer_samples_read = bytes_read / sizeof(int16_t);
    if (vector_index == 0)
    {
        // cout << "Condição 1" << endl;
        for (int a = 0; a < buffer_samples_read; a++)
        {
            buffer[a] = microfoneData[a];
        }
        vector_index = buffer_samples_read;
    }
    else if (vector_index + buffer_samples_read <= N_WINDOW_ELEMENTS)
    {
        // cout << "Condição 2" << endl;
        for (int a = 0; a < buffer_samples_read; a++)
        {
            buffer[vector_index + a] = microfoneData[a];
        }
        vector_index += buffer_samples_read;
    }
    if (vector_index + buffer_samples_read > N_WINDOW_ELEMENTS)
    {
        int valor_de_movimento = (vector_index + buffer_samples_read) % N_WINDOW_ELEMENTS;
        for (int a = 0; a < N_WINDOW_ELEMENTS - valor_de_movimento; a++)
        {
            buffer[a] = buffer[a + valor_de_movimento];
        }
        for (int a = 0; a < buffer_samples_read; a++)
        {
            buffer[N_WINDOW_ELEMENTS - valor_de_movimento + a] = microfoneData[a];
        }
        vector_index = N_WINDOW_ELEMENTS;
    }

    for (int a = 0; a < buffer_samples_read; a++)
    {
        int16_t samples = buffer[a];
        enqueue(&queueOut, samples);
    }
}

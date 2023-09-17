#include "mic.h"
#include <driver/i2s.h>
#include "esp_log.h"

#define WS_IO_PORT 22
#define SD_PORT 25
#define SCK_PORT 23
#define I2S_NUM I2S_NUM_0        // Número da interface I2S a ser usada
#define FRAME_RATE 16000          // Taxa de amostragem (16kHz)
#define N_WINDOW_ELEMENTS 1024    // Tamanho da janela de amostragem

bool mic_init() {
    i2s_config_t i2s_config = {
        .mode = I2S_MODE_MASTER | I2S_MODE_RX,
        .sample_rate = FRAME_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_MSB,
        .intr_alloc_flags = 0,
        .dma_buf_count = 16, 
        .dma_buf_len = N_WINDOW_ELEMENTS,
        .use_apll = false,
    };

    i2s_pin_config_t pin_config = {
        .bck_io_num = SCK_PORT,
        .ws_io_num = WS_IO_PORT,
        .data_out_num = I2S_PIN_NO_CHANGE,
        .data_in_num = SD_PORT,
    };

    esp_err_t err = i2s_driver_install(I2S_NUM, &i2s_config, 0, NULL);
    esp_err_t err2 = i2s_set_pin(I2S_NUM, &pin_config);
    if (err != ESP_OK || err2 != ESP_OK) {
        ESP_LOGE("MIC", "Failed to install I2S driver (%s)", esp_err_to_name(err));
        return false;
    }
    ESP_LOGI("MIC", "Microphone initialized");
    return true;
}

void mic_read_data(int16_t* buffer, int buffer_samples) {
    size_t bytes_read;
    i2s_read(I2S_NUM, buffer, buffer_samples * sizeof(int16_t), &bytes_read, portMAX_DELAY);
}

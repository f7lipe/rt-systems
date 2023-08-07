//mic.c

#include "mic.h"

#ifdef __ESP_PLATFORM__
#include "driver/i2s.h"
#else
#include <portaudio.h>
#include <stdio.h>
#endif

#define I2S_NUM I2S_NUM_0
#define I2S_SAMPLE_RATE 16000
#define I2S_READ_LEN 16
#define SAMPLE_RATE 16000

void mic_init(void) {
#ifdef __ESP_PLATFORM__
    i2s_config_t i2s_config = {
        .mode = I2S_MODE_MASTER | I2S_MODE_RX,
        .sample_rate = I2S_SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 4,
        .dma_buf_len = 1024
    };
    i2s_driver_install(I2S_NUM, &i2s_config, 0, NULL);
    i2s_set_adc_mode(ADC_UNIT_1, ADC_CHANNEL_6);
    #else

    PaError err = Pa_Initialize();
    if (err != paNoError) {
        printf("Erro ao inicializar o PortAudio: %s\n", Pa_GetErrorText(err));
        exit(1);
    }
#endif
}

void mic_read(float *buffer, int buffer_size) {
#ifdef __ESP_PLATFORM__
    // On ESP32, read from I2S
    size_t bytes_read;
    i2s_read(I2S_NUM, buffer, buffer_size * sizeof(float), &bytes_read, portMAX_DELAY);
#else
    // On computer, use PortAudio
    PaStream *stream;
    PaError err;
    PaStreamParameters inputParameters;

    inputParameters.device = 8; // Índice do dispositivo de entrada desejado

    err = Pa_OpenDefaultStream(&stream,
                               1, // Input channels (mono)
                               0, // Output channels
                               paFloat32, // Formato dos dados de áudio
                               SAMPLE_RATE,
                               buffer_size,
                               NULL, // Callback para processamento de áudio (não usado aqui)
                               NULL); // Dados adicionais do callback (não usado aqui)
    if (err != paNoError) {
        printf("Erro ao abrir o stream de áudio: %s\n", Pa_GetErrorText(err));
        Pa_Terminate();
        exit(1);
    }

    err = Pa_StartStream(stream);
    if (err != paNoError) {
        printf("Erro ao iniciar o stream de áudio: %s\n", Pa_GetErrorText(err));
        Pa_CloseStream(stream);
        Pa_Terminate();
        exit(1);
    }

    err = Pa_ReadStream(stream, buffer, buffer_size);
    if (err != paNoError) {
        printf("Erro ao ler o stream de áudio: %s\n", Pa_GetErrorText(err));
        Pa_StopStream(stream);
        Pa_CloseStream(stream);
        Pa_Terminate();
        exit(1);
    }

    Pa_StopStream(stream);
    Pa_CloseStream(stream);
#endif
}
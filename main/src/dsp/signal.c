#include "signal.h"
#include "esp_log.h"
#include <stdlib.h>
#include "esp_dsp.h"
#include <math.h>

static const char *TAG = "DSP";

#define WINDOW_SIZE 10
#define Z_SCORE_THRESHOLD 3.0
#define VREF 0.00002 // Tensão de referência em Volts (20 μPa)

float calculate_moving_average(int16_t* data, int window_size) {
    float sum = 0.0;
    for (int i = 0; i < window_size; i++) {
        sum += data[i];
    }
    float moving_avg = sum / window_size;
    return moving_avg;
}

float calculate_moving_stddev(int16_t* data, int window_size, float moving_avg) {
    float sum_sq_diff = 0.0;
    for (int i = 0; i < window_size; i++) {
        float diff = data[i] - moving_avg;
        sum_sq_diff += diff * diff;
    }
    float moving_stddev = sqrtf(sum_sq_diff / window_size);
    return moving_stddev;
}

float calculate_power_in_dB(int16_t* data, int window_size) {
    float sum = 0.0;
    for (int i = 0; i < window_size; i++) {
        sum += data[i] * data[i];
    }
    float dB = 20 * log10f(sum/VREF);
    return dB;
}

float calculate_z_score(float value, float moving_avg, float moving_stddev) {
    if (moving_stddev == 0) {
        return 0; 
    }
    return (value - moving_avg) / moving_stddev;
}

bool signal_detect_presence(int16_t* audio_window) {

    float moving_avg = calculate_moving_average(audio_window, WINDOW_SIZE);
    float moving_stddev = calculate_moving_stddev(audio_window, WINDOW_SIZE, moving_avg);
    float dB = calculate_power_in_dB(audio_window, WINDOW_SIZE);

    float z_score = calculate_z_score(dB, moving_avg, moving_stddev);

    if (fabs(z_score) > Z_SCORE_THRESHOLD) {
        ESP_LOGI(TAG, "Detected presence of outlier: power = %f, z_score = %f", dB, z_score);
        return true;
    } 
    return false;
}

#define FFT_SIZE 1024
#define SAMPLE_RATE 16000

float signal_find_higher_power_frequency(int16_t* audio_window) {
    float *power_spectrum = (float *)malloc((FFT_SIZE / 2) * sizeof(float));
    if (power_spectrum == NULL) {
        ESP_LOGE(TAG, "Memory allocation error");
        return 0.0;
    }

    esp_err_t ret = dsps_fft2r_init_fc32(NULL, FFT_SIZE);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize FFT. Error = %i", ret);
        free(power_spectrum);
        return 0.0;
    }

    float *complex_data = (float *)malloc(2 * FFT_SIZE * sizeof(float));
    if (complex_data == NULL) {
        ESP_LOGE(TAG, "Memory allocation error");
        free(power_spectrum);
        return 0.0;
    }

    for (int i = 0; i < FFT_SIZE; i++) {
        complex_data[2 * i + 0] = (float)audio_window[i];
        complex_data[2 * i + 1] = 0.0;
    }

    ret = dsps_fft2r_fc32(complex_data, FFT_SIZE);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to calculate FFT: %d", ret);
        free(complex_data);
        free(power_spectrum);
        return 0.0;
    }

    dsps_bit_rev_fc32(complex_data, FFT_SIZE);
    dsps_cplx2reC_fc32(complex_data, FFT_SIZE);

    for (int i = 0; i < FFT_SIZE / 2; i++) {
        float real = complex_data[2 * i];
        float imag = complex_data[2 * i + 1];

        power_spectrum[i] = real * real + imag * imag;
    }

    // Encontre a frequência com a maior potência
    float max_power = 0.0;
    int max_power_index = 0;
    for (int i = 1; i < FFT_SIZE / 2; i++) {
        if (power_spectrum[i] > max_power) {
            max_power = power_spectrum[i];
            max_power_index = i;
        }
    }

    float frequency = ((float)max_power_index * SAMPLE_RATE) / FFT_SIZE;

    // Registro de log para indicar a frequência encontrada
    ESP_LOGI(TAG, "Higher power frequency found: %.2f Hz", frequency);

    // Libere a memória alocada
    free(complex_data);
    free(power_spectrum);

    return frequency;
}

void signal_modify_velocity(int16_t* audio_window, int signalVelocity) {
    // Verifique se a velocidade é válida (1x, 2x ou 4x)
    if (signalVelocity != 1 && signalVelocity != 2 && signalVelocity != 4) {
        ESP_LOGE(TAG, "Invalid signal velocity: %d", signalVelocity);
        return;
    }

    // Tamanho da janela de entrada (ajuste conforme necessário)
    int input_window_size = FFT_SIZE; // Suponha que seja igual ao tamanho da FFT

    // Tamanho da janela de saída após a modificação da velocidade
    int output_window_size = input_window_size / signalVelocity;

    // Aloque memória para a janela de saída modificada
    int16_t *output_window = (int16_t *)malloc(output_window_size * sizeof(int16_t));
    if (output_window == NULL) {
        ESP_LOGE(TAG, "Memory allocation error");
        return;
    }

    // Realize a interpolação linear para modificar a velocidade
    for (int i = 0; i < output_window_size; i++) {
        // Calcule o índice correspondente na janela de entrada
        int input_index = i * signalVelocity;

        // Use interpolação linear para calcular o valor na janela de saída
        if (input_index >= input_window_size - 1) {
            output_window[i] = audio_window[input_index];
        } else {
            // Parte fracionária do índice
            float fraction = (float)(i * signalVelocity) - input_index;

            // Interpolação linear
            output_window[i] = (int16_t)((1.0 - fraction) * audio_window[input_index] +
                                          fraction * audio_window[input_index + 1]);
        }
    }

    // Agora, a janela de saída modificada contém o sinal com a velocidade alterada
    // Você pode usar output_window conforme necessário no seu projeto

    // Libere a memória alocada
    free(output_window);
}
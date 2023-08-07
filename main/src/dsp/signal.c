// signal.c

#include "signal.h"
#include "esp_dsp.h"
#include <math.h>

static float window[FFT_SIZE];
static float complex_output[FFT_SIZE * 2];

void signal_init(void) {
    dsps_fft2r_init_fc32(NULL, CONFIG_DSP_MAX_FFT_SIZE);
    dsps_wind_hann_f32(window, FFT_SIZE);
}

void process_signal(float *input_signal, float *output_signal, int signal_length) {
    if (signal_length != FFT_SIZE) {
        // Error handling: Input signal length must be equal to FFT_SIZE
        return;
    }

    // Apply windowing to the input signal
    for (int i = 0; i < FFT_SIZE; i++) {
        input_signal[i] *= window[i];
    }

    // Convert the real input signal to a complex array
    for (int i = 0; i < FFT_SIZE; i++) {
        complex_output[i * 2 + 0] = input_signal[i];
        complex_output[i * 2 + 1] = 0.0;
    }

    // Perform FFT
    dsps_fft2r_fc32(complex_output, FFT_SIZE);

    // Bit reverse
    dsps_bit_rev_fc32(complex_output, FFT_SIZE);

    // Convert the complex output to two real arrays
    dsps_cplx2reC_fc32(complex_output, FFT_SIZE);

    // Copy the processed signal to the output buffer
    for (int i = 0; i < FFT_SIZE / 2; i++) {
        output_signal[i] = 10 * log10f((complex_output[i * 2 + 0] * complex_output[i * 2 + 0] +
                                        complex_output[i * 2 + 1] * complex_output[i * 2 + 1]) / FFT_SIZE);
    }
}

int detect_presence(float *signal, int signal_length, float noise_threshold) {
    // Detect presence of a signal with power higher than the noise_threshold

    for (int i = 0; i < signal_length; i++) {
        if (signal[i] > noise_threshold) {
            return 1; // Signal detected
        }
    }

    return 0; // Signal not detected
}

float calculate_peak_frequency(float *signal, int signal_length, float sample_rate) {
    // Calculate the peak frequency from the signal
    // For example, find the bin with the maximum magnitude and calculate the corresponding frequency

    float max_magnitude = 0.0;
    int max_index = 0;

    for (int i = 0; i < signal_length; i++) {
        if (signal[i] > max_magnitude) {
            max_magnitude = signal[i];
            max_index = i;
        }
    }

    // Calculate the peak frequency in Hz
    float peak_frequency = (float)max_index * sample_rate / signal_length;

    return peak_frequency;
}
// signal.h

#ifndef SIGNAL_H
#define SIGNAL_H

#include <stdint.h>

#define FFT_SIZE 1024

void signal_init(void);
void process_signal(float *input_signal, float *output_signal, int signal_length);
int detect_presence(float *signal, int signal_length, float noise_threshold);
float calculate_peak_frequency(float *signal, int signal_length, float sample_rate);

#endif  // SIGNAL_H

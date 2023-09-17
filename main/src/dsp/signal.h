#ifndef SIGNAL_H
#define SIGNAL_H

#include <stdint.h>
#include <stdbool.h>

bool signal_detect_presence(int16_t* window);
void signal_modify_velocity(int16_t* windowModified, int window_samples, int signalVelocity);
void signal_mix_with_music(int16_t *audio_data, int32_t len);
float signal_find_higher_power_frequency(int16_t* window);

#endif /* SIGNAL_H */
#ifndef AUDIO_UTILS_H
#define AUDIO_UTILS_H

#include <stdint.h>
#include <stddef.h>

void audio_utils_init();
void audio_utils_load_wav(const char* filename, int16_t** audio_data, size_t* audio_size);

#endif

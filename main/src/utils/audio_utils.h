#ifndef AUDIO_UTILS_H
#define AUDIO_UTILS_H

#include <stdint.h>
#include <stddef.h>

#define AUDIO_DATA_SIZE 1024 // Tamanho dos buffers de áudio

typedef struct {
    int16_t real_data[AUDIO_DATA_SIZE];
    int16_t simulated_data[AUDIO_DATA_SIZE];
} AudioData;

extern AudioData audio_data;

#endif

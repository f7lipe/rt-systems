#ifndef MIC_H
#define MIC_H

#include <stdint.h>

void mic_init();
void mic_read_data(int16_t* buffer, int buffer_samples);

#endif /* MIC_H */

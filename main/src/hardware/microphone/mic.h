// mic.h

#ifndef MIC_H
#define MIC_H

#include <stdint.h>

void mic_init(void);
void mic_read(float *buffer, int buffer_size);

#endif  // MIC_H


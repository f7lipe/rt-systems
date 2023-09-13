// bluetooth.h

#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include "esp_a2dp_api.h"
#include <stdbool.h>

// Inicialize o módulo Bluetooth
bool bluetooth_init();

// Envie dados de áudio Bluetooth
void bluetooth_send_audio_data(uint8_t *data, size_t len);

// Limpeza do módulo Bluetooth
void bluetooth_cleanup();

#endif // BLUETOOTH_H

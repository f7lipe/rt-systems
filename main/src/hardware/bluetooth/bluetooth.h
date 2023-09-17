// bluetooth.h

#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include "esp_a2dp_api.h"
#include <stdbool.h>

// Inicialize o módulo Bluetooth
bool bluetooth_init();

// Envie dados de áudio Bluetooth


// Verifique se o dispositivo Bluetooth está conectado
bool bluetooth_is_connected();

// Limpeza do módulo Bluetooth
void bluetooth_cleanup();

#endif // BLUETOOTH_H

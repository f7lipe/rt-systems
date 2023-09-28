#include "esp_log.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "src/hardware/bluetooth/bluetooth.h"
#include "src/hardware/microphone/mic.h"
#include "src/hardware/button/buttons.h"
#include "src/utils/audio_utils.h"
#include "src/dsp/signal.h"
#include "src/ui/ui.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#define N_WINDOW_ELEMENTS 1024
int signalVelocity = 1;
int16_t buffer[N_WINDOW_ELEMENTS];

// Declaração das variáveis globais para presence e higherPowerFrequency
bool presence;
float higherPowerFrequency;

// Defina as tarefas
void tarefaLeituraMicrofone(void *param) {
    while (1) {
        if (bluetooth_is_connected()) {
            mic_read_data(buffer, N_WINDOW_ELEMENTS);


                for (int i = 0; i < N_WINDOW_ELEMENTS; i++)
                {
                     audio_data.real_data[i] = buffer[i];
                }
        }
        vTaskDelay(pdMS_TO_TICKS(64)); // Período de leitura do microfone: 64 ms
    }
}

void tarefaModificacaoVelocidade(void *param) {
    while (1) {
        if (bluetooth_is_connected()) {
            signal_modify_velocity(buffer, N_WINDOW_ELEMENTS, signalVelocity);
        }
        vTaskDelay(pdMS_TO_TICKS(128)); // Período de modificação da velocidade: 128 ms
    }
}

void tarefaDetecaoPresencaSinal(void *param) {
    while (1) {
        if (bluetooth_is_connected()) {
            presence = signal_detect_presence(buffer);
            // Faça algo com a detecção de presença
        }
        vTaskDelay(pdMS_TO_TICKS(128)); // Período de detecção de presença de sinal: 128 ms
    }
}

void tarefaEncontrarFrequencia(void *param) {
    while (1) {
        if (bluetooth_is_connected()) {
            higherPowerFrequency = signal_find_higher_power_frequency(buffer);
            // Faça algo com a frequência encontrada
        }
        vTaskDelay(pdMS_TO_TICKS(64)); // Período de encontrar a frequência com maior potência: 64 ms
    }
}

void tarefaAtualizacaoDisplay(void *param) {
    while (1) {
        if (bluetooth_is_connected()) {
            ui_update_display(higherPowerFrequency);
        }
        vTaskDelay(pdMS_TO_TICKS(256)); // Período de atualização do display: 256 ms
    }
}

void tarefaAtualizacaoLED(void *param) {
    while (1) {
        if (bluetooth_is_connected()) {
            ui_update_led(presence);
        }
        vTaskDelay(pdMS_TO_TICKS(512)); // Período de atualização do LED: 512 ms
    }
}

void update_button_state()
{
    if (button_is_pressed())
    {
        signalVelocity = (signalVelocity % 4) + 1;
        ESP_LOGI("BUTTON", "Button pressed, velocity is now %dx", signalVelocity);
    }
}

void app_main(void)
{
    // FIFO
    QueueHandle_t filaTarefas = xQueueCreate(10, sizeof(void *));

    if (mic_init() && ui_init() && bluetooth_init())
    {
        // TODO: Criar as tarefas na CPU 1
        xTaskCreate(tarefaLeituraMicrofone, "LeituraMicrofone", 2048, NULL, 1, NULL);
        xTaskCreate(tarefaModificacaoVelocidade, "ModificacaoVelocidade", 2048, NULL, 1, NULL);
        xTaskCreate(tarefaDetecaoPresencaSinal, "DetecaoPresencaSinal", 2048, NULL, 1, NULL);
        xTaskCreate(tarefaEncontrarFrequencia, "EncontrarFrequencia", 2048, NULL, 1, NULL);
        xTaskCreate(tarefaAtualizacaoDisplay, "AtualizacaoDisplay", 2048, NULL, 1, NULL);
        xTaskCreate(tarefaAtualizacaoLED, "AtualizacaoLED", 2048, NULL, 1, NULL);

        // TODO: Definir o período de cada tarefa
        const TickType_t periodos[] = {
            pdMS_TO_TICKS(64),  // Leitura do Microfone: 64 ms
            pdMS_TO_TICKS(128), // Modificação da Velocidade: 128 ms
            pdMS_TO_TICKS(128), // Detecção de Presença de Sinal: 128 ms
            pdMS_TO_TICKS(64),  // Encontrar Frequência com Maior Potência: 64 ms
            pdMS_TO_TICKS(256), // Atualização do Display: 256 ms
            pdMS_TO_TICKS(512)  // Atualização do LED: 512 ms
        };

        int i = 0;
        while (1)
        {
            // Add próxima tarefa na fila
            xQueueSendToBack(filaTarefas, &i, portMAX_DELAY);

            vTaskDelay(periodos[i]);

            i = (i + 1) % 6; // Alternar enre as 6 tarefas 
        }
        bluetooth_cleanup();
    }
}

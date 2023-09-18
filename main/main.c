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
#include <time.h>

// Definir uma estrutura para armazenar informações da tarefa
struct TaskInfo
{
    const char *name;      // Nome da tarefa
    void (*task)();        // Função da tarefa
    double execution_time; // Tempo de execução
};

// Declarações
#define N_WINDOW_ELEMENTS 1024
int signalVelocity = 1;
int16_t buffer[N_WINDOW_ELEMENTS];

bool init(void)
{
    return mic_init() /*&& bluetooth_init()*/ && ui_init();
}

void deinit(void)
{
    //bluetooth_cleanup();
    ui_update_led(0);
}

void read_data()
{
     mic_read_data(buffer, N_WINDOW_ELEMENTS);
}

void modify_velocity()
{
    signal_modify_velocity(buffer, N_WINDOW_ELEMENTS, signalVelocity);
}

void detect_presence()
{
    signal_detect_presence(buffer);
}

void higher_power_frequency()
{
    signal_find_higher_power_frequency(buffer);
}

void update_display()
{
     ui_update_display(343.1);
}

void update_led()
{
    ui_update_led(1);
}

void app_main(void)
{

    if (init())
    {
        ESP_LOGI("MAIN", "Initialization successful");

        #define N_REPETITIONS 10
        struct TaskInfo tasks[] = {
            {"Leitura do Microfone", read_data, 0.0},
            {"Modificação da Velocidade do Sinal", modify_velocity, 0.0},
            {"Detecção de Presença de Sinal", detect_presence, 0.0},
            {"Encontrar a Frequência com Maior Potência", higher_power_frequency, 0.0},
            {"Atualização do Display", update_display, 0.0},
            {"Atualização do LED", update_led, 0.0}};

        int num_tasks = sizeof(tasks) / sizeof(tasks[0]);

        for (int i = 0; i < num_tasks; i++)
        {
            clock_t start_time, end_time;

            start_time = clock();
            for (int j = 0; j < N_REPETITIONS; j++)
            {
                tasks[i].task();
                signalVelocity++;
            }
            end_time = clock();

            tasks[i].execution_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
        }

        printf("Tarefa\t\tTempo médio de execução\n");
        for (int i = 0; i < num_tasks; i++)
        {
            printf("%s\t\t%.3f s\n", tasks[i].name, tasks[i].execution_time/N_REPETITIONS);
        }
    }
    else
    {
        ESP_LOGE("MAIN", "Initialization failed");
        return;
    }

    deinit();

}

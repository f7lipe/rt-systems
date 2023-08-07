#include "src/ui/ui.h"
#include "src/hardware/microphone/mic.h"
#include "src/dsp/signal.h"

#define BUFFER_SIZE 1024
#define SAMPLE_RATE 16000

void app_main(void) {
    ui_init();

    mic_init();

    float mic_buffer[BUFFER_SIZE];
    float processed_data[BUFFER_SIZE];

    while (1) {

        mic_read(mic_buffer, BUFFER_SIZE);

        process_signal(mic_buffer, processed_data, BUFFER_SIZE);

        int signal_present = detect_presence(processed_data, BUFFER_SIZE, 0.1);
        ui_show_signal_presence(signal_present);
        
        float frequency = calculate_peak_frequency(processed_data, BUFFER_SIZE, SAMPLE_RATE);
        ui_display_frequency(frequency);

        // TODO - Implementar próximas tarefas do projeto
        // ...
    }
}

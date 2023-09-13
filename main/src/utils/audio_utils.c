#include "audio_utils.h"
#include "esp_log.h"
#include "esp_spiffs.h"

#define TAG "AUDIO_UTILS"

void audio_utils_init() {
    esp_vfs_spiffs_conf_t spiffs_config = {
        .base_path = "/spiffs",
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = true,
    };

    esp_err_t ret = esp_vfs_spiffs_register(&spiffs_config);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Falha ao montar o sistema de arquivos SPIFFS (%s)", esp_err_to_name(ret));
    }
}

void audio_utils_load_wav(const char* filename, int16_t** audio_data, size_t* audio_size) {
    FILE* wav_file = fopen(filename, "rb");
    if (wav_file == NULL) {
        ESP_LOGE(TAG, "Falha ao abrir o arquivo WAV (%s)", filename);
        return;
    }

    // Obtenha o tamanho do arquivo WAV
    fseek(wav_file, 0, SEEK_END);
    long file_size = ftell(wav_file);
    fseek(wav_file, 0, SEEK_SET);

    if (file_size <= 0) {
        ESP_LOGE(TAG, "Tamanho de arquivo WAV inválido.");
        fclose(wav_file);
        return;
    }

    // Aloque memória para armazenar os dados do arquivo WAV
    int16_t* data = (int16_t*)malloc(file_size);
    if (data == NULL) {
        ESP_LOGE(TAG, "Falha na alocação de memória para dados WAV.");
        fclose(wav_file);
        return;
    }

    // Leia os dados do arquivo WAV
    size_t bytes_read = fread(data, 1, file_size, wav_file);
    fclose(wav_file);

    if (bytes_read != (size_t)file_size) {
        ESP_LOGE(TAG, "Erro ao ler dados do arquivo WAV.");
        free(data);
        return;
    }

    *audio_data = data;
    *audio_size = bytes_read;
}
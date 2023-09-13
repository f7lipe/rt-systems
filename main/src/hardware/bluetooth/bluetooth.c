// bluetooth.c
#include "bluetooth.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_gap_ble_api.h"
#include "esp_a2dp_api.h"
#include "esp_bt_device.h"
#include "esp_err.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_gap_bt_api.h"
#include "esp_avrc_api.h"

#define BT_TAG "BLUETOOTH"

static bool bluetooth_initialized = false;

static void bt_app_gap_cb(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param) {
    switch (event) {
        case ESP_BT_GAP_DISC_RES_EVT: {
            // Um dispositivo Bluetooth foi descoberto
            // Você pode verificar os detalhes do dispositivo em param->disc_res
            ESP_LOGI(BT_TAG, "Dispositivo Bluetooth descoberto");
            break;
        }
        // Outros casos de eventos podem ser tratados aqui
        default:
            break;
    }
}


bool bluetooth_init() {

    // Inicializar o NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Inicializar o controlador Bluetooth
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();


    if (esp_bt_controller_init(&bt_cfg) != ESP_OK) {
        ESP_LOGE(BT_TAG, "Falha ao inicializar o controlador Bluetooth");
        return false;
    }

    if (esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT) != ESP_OK) {
        ESP_LOGE(BT_TAG, "Falha ao habilitar o controlador Bluetooth");
        return false;
    }


    if (esp_bluedroid_init() != ESP_OK && esp_bluedroid_enable() != ESP_OK) {
        ESP_LOGE(BT_TAG, "Falha ao habilitar o Bluedroid");
        return false;
    }

    esp_bt_dev_set_device_name("ESP32-A2DP-SINK");
    esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE);
    esp_bt_gap_start_discovery(ESP_BT_INQ_MODE_GENERAL_INQUIRY, 10, 0);

    esp_bt_gap_register_callback(bt_app_gap_cb);


    bluetooth_initialized = true;

    if (bluetooth_initialized) {
        ESP_LOGI(BT_TAG, "Bluetooth inicializado com sucesso");
        return true;
    }
    return false;
}

void bluetooth_send_audio_data(uint8_t *data, size_t len) {
    if (!bluetooth_initialized) {
        ESP_LOGE(BT_TAG, "Bluetooth não ini´cializado. Não é possível enviar dados de áudio.");
        return;
    }

}

void bluetooth_cleanup() {
    if (!bluetooth_initialized) {
        ESP_LOGE(BT_TAG, "Bluetooth não inicializado. Nada a limpar.");
        return;
    }

    // Desativar o perfil A2DP e o stack Bluetooth
    esp_a2d_sink_deinit();
    esp_bluedroid_disable();
    esp_bluedroid_deinit();
    esp_bt_controller_disable();
    esp_bt_controller_deinit();
    bluetooth_initialized = false;
}



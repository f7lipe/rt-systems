// display.c

#include "display.h"
#include <stdio.h>

#ifdef __ESP_PLATFORM__ 
#include "driver/gpio.h"
#endif

#define DIGIT1_PIN 0
#define DIGIT2_PIN 1
#define DIGIT3_PIN 2
#define DIGIT4_PIN 3
#define SEG_A_PIN 4
#define SEG_B_PIN 5
#define SEG_C_PIN 6
#define SEG_D_PIN 7
#define SEG_E_PIN 8
#define SEG_F_PIN 9
#define SEG_G_PIN 10

#ifdef __ESP_PLATFORM__ 
static const uint8_t digit_values[] = {
    0b00111111,  // 0
    0b00000110,  // 1
    0b01011011,  // 2
    0b01001111,  // 3
    0b01100110,  // 4
    0b01101101,  // 5
    0b01111101,  // 6
    0b00000111,  // 7
    0b01111111,  // 8
    0b01101111   // 9
};
#endif

void display_init(void) {
#ifdef __ESP_PLATFORM__ 
    gpio_set_direction(DIGIT1_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(DIGIT2_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(DIGIT3_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(DIGIT4_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(SEG_A_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(SEG_B_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(SEG_C_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(SEG_D_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(SEG_E_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(SEG_F_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(SEG_G_PIN, GPIO_MODE_OUTPUT);
#else 
    printf("Display inicializado no computador.\n");
#endif
}

void display_show_digit(uint8_t digit, uint8_t value) {
#ifdef __ESP_PLATFORM__
    gpio_set_level(DIGIT1_PIN, 0);
    gpio_set_level(DIGIT2_PIN, 0);
    gpio_set_level(DIGIT3_PIN, 0);
    gpio_set_level(DIGIT4_PIN, 0);

    gpio_set_level(SEG_A_PIN, 0);
    gpio_set_level(SEG_B_PIN, 0);
    gpio_set_level(SEG_C_PIN, 0);
    gpio_set_level(SEG_D_PIN, 0);
    gpio_set_level(SEG_E_PIN, 0);
    gpio_set_level(SEG_F_PIN, 0);
    gpio_set_level(SEG_G_PIN, 0);

    gpio_set_level(DIGIT1_PIN + digit, 1);
    uint8_t segment_mask = digit_values[value];
    if (segment_mask & (1 << 0)) gpio_set_level(SEG_A_PIN, 1);
    if (segment_mask & (1 << 1)) gpio_set_level(SEG_B_PIN, 1);
    if (segment_mask & (1 << 2)) gpio_set_level(SEG_C_PIN, 1);
    if (segment_mask & (1 << 3)) gpio_set_level(SEG_D_PIN, 1);
    if (segment_mask & (1 << 4)) gpio_set_level(SEG_E_PIN, 1);
    if (segment_mask & (1 << 5)) gpio_set_level(SEG_F_PIN, 1);
    if (segment_mask & (1 << 6)) gpio_set_level(SEG_G_PIN, 1);
#else 
    printf("Mostrando dígito %d com valor %d no computador.\n", digit, value);
#endif
}

void display_clear(void) {
#ifdef __ESP_PLATFORM__
    gpio_set_level(DIGIT1_PIN, 0);
    gpio_set_level(DIGIT2_PIN, 0);
    gpio_set_level(DIGIT3_PIN, 0);
    gpio_set_level(DIGIT4_PIN, 0);
    gpio_set_level(SEG_A_PIN, 0);
    gpio_set_level(SEG_B_PIN, 0);
    gpio_set_level(SEG_C_PIN, 0);
    gpio_set_level(SEG_D_PIN, 0);
    gpio_set_level(SEG_E_PIN, 0);
    gpio_set_level(SEG_F_PIN, 0);
    gpio_set_level(SEG_G_PIN, 0);
#else
    printf("Limpando display no computador.\n");
#endif
}
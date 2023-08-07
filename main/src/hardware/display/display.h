// display.h

#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

void display_init(void);
void display_show_digit(uint8_t digit, uint8_t value);
void display_clear(void);

#endif  // DISPLAY_H
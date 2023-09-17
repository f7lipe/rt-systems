#ifndef UI_H
#define UI_H

#include <stdbool.h>

bool ui_init();
void ui_update_display(float frequency);
void ui_update_led(int signal);

#endif /* UI_H */
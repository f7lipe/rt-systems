#ifndef BUTTON_H
#define BUTTON_H

// Defina o número de botões que você está usando
#define NUM_BUTTONS 3

// Enumeração para representar o estado dos botões
typedef enum {
    BUTTON_STATE_IDLE,
    BUTTON_STATE_PRESSED,
    BUTTON_STATE_RELEASED
} button_state_t;

// Função de inicialização dos botões
void button_init();

// Função para ler o estado de um botão específico
button_state_t button_read(int button_index);

#endif
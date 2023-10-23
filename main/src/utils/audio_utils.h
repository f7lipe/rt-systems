#ifndef AUDIO_UTILS_H
#define AUDIO_UTILS_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_SIZE_QUEUE 1024 // Tamanho máximo da fila (ajuste conforme necessário)

typedef struct {
    int16_t items[MAX_SIZE_QUEUE];
    int front;
    int rear;
} Queue;

typedef struct {
    int16_t real_data[MAX_SIZE_QUEUE];
    int16_t simulated_data[MAX_SIZE_QUEUE];
} AudioData;

void initialize(Queue *queue);
bool isEmpty(Queue *queue);
bool isFull(Queue *queue);
void enqueue(Queue *queue, int16_t value);
void dequeue(Queue *queue);
int16_t peek(Queue *queue);
int sizeQueue(Queue *queue);

extern int16_t buffer[];
extern AudioData audio_data;
extern Queue queueOut;


#endif

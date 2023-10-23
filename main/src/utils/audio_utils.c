#include "audio_utils.h"

AudioData audio_data;

void initialize(Queue *queue) {
    queue->front = -1;
    queue->rear = -1;
}

bool isEmpty(Queue *queue) {
    return (queue->front == -1 && queue->rear == -1);
}

bool isFull(Queue *queue) {
    return (queue->rear + 1) % MAX_SIZE_QUEUE == queue->front;
}

void enqueue(Queue *queue, int16_t value) {
    if (isFull(queue)) {
        dequeue(queue);
    }

    if (isEmpty(queue)) {
        queue->front = queue->rear = 0;
    } else {
        queue->rear = (queue->rear + 1) % MAX_SIZE_QUEUE;
    }
    queue->items[queue->rear] = value;
}

void dequeue(Queue *queue) {
    if (isEmpty(queue)) {
    } else {
        if (queue->front == queue->rear) {
            queue->front = queue->rear = -1;
        } else {
            queue->front = (queue->front + 1) % MAX_SIZE_QUEUE;
        }
    }
}

int16_t peek(Queue *queue) {
    if (isEmpty(queue)) {
        return -1;
    } else {
        return queue->items[queue->front];
    }
}

int sizeQueue(Queue *queue) {
    if (isEmpty(queue)) {
        return 0;
    } else {
        return (queue->rear - queue->front + 1 + MAX_SIZE_QUEUE) % MAX_SIZE_QUEUE;
    }
}
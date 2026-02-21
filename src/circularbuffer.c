#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define BUFFER_SIZE 10

typedef struct {
    int data[BUFFER_SIZE];
    int head;
    int tail;
} CircularBuffer;

typedef struct {
    float data[BUFFER_SIZE];
    int head;
    int tail;
} CircularBufferFloat;

CircularBuffer initCircularBuffer() {
    return (CircularBuffer) {.head = 0, .tail = 0};
}

void pushtoCircularBuffer(CircularBuffer *cb, int data) {
    if ((cb->head + 1) % BUFFER_SIZE == cb->tail) {
        printf("Buffer full\n");
        return;
    }
    cb->data[cb->head] = data;
    cb->head = (cb->head + 1) % BUFFER_SIZE;
}

int popfromCircularBuffer(CircularBuffer *cb) {
    if (cb->head == cb->tail) {
        printf("Buffer empty\n");
        return -1;
    }
    int data = cb->data[cb->tail];
    cb->tail = (cb->tail + 1) % BUFFER_SIZE;
    return data;
}

void copyToCircularBuffer(CircularBuffer *cb, int *array, size_t length) {
    if (length > BUFFER_SIZE) length = BUFFER_SIZE;
    memcpy(cb->data, array, length);
}

int peekCircularBuffer(CircularBuffer *cb) {
    return cb->data[cb->tail];
}




CircularBufferFloat initCircularBufferFloat() {
    return (CircularBufferFloat) {.head = 0, .tail = 0};
}

void pushtoCircularBufferFloat(CircularBufferFloat *cb, float data) {
    if ((cb->head + 1) % BUFFER_SIZE == cb->tail) {
        printf("Buffer full\n");
        return;
    }
    cb->data[cb->head] = data;
    cb->head = (cb->head + 1) % BUFFER_SIZE;
}

float popfromCircularBufferFloat(CircularBufferFloat *cb) {
    if (cb->head == cb->tail) {
        printf("Buffer empty\n");
        return -1;
    }
    float data = cb->data[cb->tail];
    cb->tail = (cb->tail + 1) % BUFFER_SIZE;
    return data;
}

void copyToCircularBufferFloat(CircularBufferFloat *cb, float *array, size_t length) {
    if (length > BUFFER_SIZE) length = BUFFER_SIZE;
    memcpy(cb->data, array, length);
}

float peekCircularBufferFloat(CircularBufferFloat *cb) {
    return cb->data[cb->tail];
}



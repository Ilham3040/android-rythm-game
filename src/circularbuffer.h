
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


CircularBuffer initCircularBuffer();

void pushtoCircularBuffer(CircularBuffer *cb, int data);

int popfromCircularBuffer(CircularBuffer *cb);

void copyToCircularBuffer(CircularBuffer *cb, int *array, size_t length);

int peekCircularBuffer(CircularBuffer *cb);


CircularBufferFloat initCircularBufferFloat();

void pushtoCircularBufferFloat(CircularBufferFloat *cb, float data);

float popfromCircularBufferFloat(CircularBufferFloat *cb);

void copyToCircularBufferFloat(CircularBufferFloat *cb, float *array, size_t length);

float peekCircularBufferFloat(CircularBufferFloat *cb);
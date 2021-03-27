#ifndef WAVREGISTRY_H
#define WAVREGISTRY_H

#include <WaveHC.h>
#include <WaveUtil.h>
#include "Helper.h"

#ifndef DEBUG
#define DEBUG true  // Do debug messages? Set to false for production
#endif

// Registry of ID-filename pairs
typedef struct WAVRegistry_t {
    char name[11];
    uint8_t* size;
    struct WAVRegistry_t* next;
} WAVRegistry;

// Functions
void generateRegistry(WAVRegistry** registry);

#endif
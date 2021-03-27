#include <WaveHC.h>
#include <WaveUtil.h>
#include "Helper.h"
#include "WAVRegistry.h"

// Function definitions
void play(FatReader &dir);

WAVRegistry* registry;

void setup() {
    Serial.begin(9600);

    putstring_nl("\nPOST:");
    post();

    putstring_nl("\nGenerating registry...");
    generateRegistry(&registry);

    Serial.println();
    Serial.print(*(registry->size));
    putstring_nl(" WAV files found:");

    for(int i = 0; i < *(registry->size); i++){
        Serial.print(String(i)+" - "+String(registry->name));
        registry = registry->next;
    }
}

void loop() {
    
}
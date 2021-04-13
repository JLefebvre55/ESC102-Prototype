#include <WaveHC.h>
#include <WaveUtil.h>
#include "Helper.h"
#include "WAVRegistry.h"

// Function definitions
void play(FatReader &dir);

WAVRegistry* registry;

void setup() {
    Serial.begin(115200);

    PgmPrintln("POST");
    post();

    PgmPrintln("\nRegistry");
    generateRegistry(&registry);

    #if DEBUG
    debug("Registry loaded (");
    Serial.print(*(registry->size));
    PgmPrintln(" files):");
    for(int i = 0; i < *(registry->size); i++){
        Serial.println(String(i+1)+" - "+String(registry->name));
        registry = registry->next;
    }
    #endif
}

void loop() {
    
}

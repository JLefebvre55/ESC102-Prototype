#include "Helper.h"

// Declare globals
uint32_t t0;
SdReader card;
FatVolume vol;
FatReader root;
WaveHC wave;

// Print a flash-mem string with ERROR prefix, and halt
void fail_P(const char *str = "") {
    PgmPrint("[ERROR - ");
    Serial.print(TIMESTR);
    PgmPrint("] ");
    SerialPrint_P(str);
    while(1);
}

// Print a flash-mem string with the WARN prefix
void warn_P(const char* str = ""){
    PgmPrint("[WARN - ");
    Serial.print(TIMESTR);
    PgmPrint("] ");
    SerialPrint_P(str);
}

// Print a flash-mem string with DEBUG prefix
void debug_P(const char *str = "") {
    PgmPrint("[DEBUG - ");
    Serial.print(TIMESTR);
    PgmPrint("] ");
    SerialPrint_P(str);
}

void sdErrorCheck(void) {
    if (!card.errorCode()) return;
    PgmPrint(" - SD I/O error: ");
    Serial.print(card.errorCode(), HEX);
    PgmPrint(", ");
    Serial.println(card.errorData(), HEX);
}

// Initialize and POST
void post(void){
    t0 = millis();
    
    #if DEBUG
    debug("Free RAM (bytes): ");
    Serial.println(FreeRam());
    #endif

    // Initialize card
    if (!card.init()) {
        fail("Failed to initialize card!");
    }

    // enable optimized read - some cards may timeout
    card.partialBlockRead(true);

    // Find FAT partition, print info
    findFAT();

    // Initialize directory
    if (!root.openRoot(vol)) {
        fail("Failed to open root directory!");      // Something went wrong,
    }
}

// Find a FAT partition on the given card
void findFAT(void){
    uint8_t part;
    for (part = 0; part < 5; part++) {  // up to 5 slots to check
        if (vol.init(card, part)) 
            break;  // Partition found
    }
    if (part == 5) {
        fail("No valid FAT partition!");
    }

    #if DEBUG
    debug("Using FAT partition ");
    Serial.print(part, DEC);
    PgmPrint(" - Type ");
    Serial.println(vol.fatType(), DEC);     // FAT16 or FAT32?
    #endif
}

void playFile(dir_t path){

    // Search only root directory by name

    // Decode wave (assume good cause it's from the registry)

    // wave.play(); // make some noise!

    // uint8_t n = 0;
    // while (wave.isplaying) {    // playing occurs in interrupts, so we print dots in realtime
    //     // putstring(".");
    //     // if (!(++n % 32)) Serial.println();
    //     delay(100);
    // }       
    // sdErrorCheck();
    // if (wave.errors)
    //     warn_P("Error reading WAV '");
    //     printEntryName(path);
    //     Serial.print("': ")
    //     Serial.println(wave.errors);     // wave decoding errors
    // }
}
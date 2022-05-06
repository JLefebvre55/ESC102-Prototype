#include "Helper.h"

// SD
uint32_t t0;
SdReader card;
FatVolume vol;
FatReader root;
WaveHC wave;

// NFC
// Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);
uint8_t success;
uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

// Print a flash-mem string with ERROR prefix, and halt
void fail_P(const char *str = "") {
    PgmPrint("[ERROR - ");
    Serial.print(TIMESTR);
    PgmPrint("] ");
    SerialPrint_P(str);
    PgmPrint("\nPrinting root directory:\n");
    printDir(root);
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

    //------ SD CARD
    
    #if DEBUG
    debug("Free RAM (bytes): ");
    Serial.println(FreeRam());
    #endif

    // Initialize card
    if (!card.init()) {
        fail("Failed to initialize SD card!");
    }

    // enable optimized read - some cards may timeout
    card.partialBlockRead(true);

    // Find FAT partition, print info
    findFAT();

    // Initialize directory
    if (!root.openRoot(vol)) {
        fail("Failed to open root directory!");      // Something went wrong,
    }

    //------ NFC READER/WRITER

    nfc.begin();

    uint32_t versiondata = nfc.getFirmwareVersion();
    if (!versiondata) {
        fail("Failed to initialize NFC reader/writer! Check wiring.");
    }

    #if DEBUG
    // Got ok data, print it out!
    debug("Found chip PN5");
    Serial.println((versiondata>>24) & 0xFF, HEX); 
    PgmPrint("Firmware ver. ");
    Serial.print((versiondata>>16) & 0xFF, DEC); 
    PgmPrint('.');
    Serial.println((versiondata>>8) & 0xFF, DEC);
    #endif

    nfc.SAMConfig();
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

void printDir(FatReader &dir) {
  FatReader file;
  dir_t dirBuf;
  while (dir.readDir(dirBuf) > 0) {    // Read every file in the directory one at a time
    printEntryName(dirBuf);
    Serial.println();
  }
}

void checkNFC(uint8_t* data, uint8_t* success){
    uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
    uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
    
    // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
    // 'uid' will be populated with the UID, and uidLength will indicate
    // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
    uint8_t _success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

    if (_success) {
        // Display some basic information about the card
        #if DEBUG
        debug("Found an ISO14443A card - UID Length: ");
        Serial.print(uidLength, DEC);
        PgmPrint(" bytes, UID Value: ");
        nfc.PrintHex(uid, uidLength);
        Serial.println("");
        #endif

        // if (uidLength == 4) {
        #if DEBUG
        debug("Seems to be a Mifare Classic card (4 byte UID) - Trying to authenticate block 4 with default KEYA value...\n");
        #endif

        uint8_t keya[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

        // Start with block 4 (the first block of sector 1)
        // Sector 0 just contains the manufacturer data
        _success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, keya);

        if (_success) {
            #if DEBUG
            debug("Sector 1 (Blocks 4..7) has been authenticated\n");
            #endif

            // Test write
            // int num = rand()%100;
            // #if DEBUG
            // debug("Writing a random number to block 4: ");
            // Serial.print("0x");
            // Serial.println(num, 16);
            // #endif

            uint8_t data[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0, 0, 0, 0, 0, 0};
            data[0] = num;
            success = nfc.mifareclassic_WriteDataBlock(4, data);

            if(success){
                // Try to read the contents of block 4
                success = nfc.mifareclassic_ReadDataBlock(4, data);

                if (success) {
                    // Data seems to have been read ... spit it out
                    Serial.println("Reading Block 4:");
                    for(int i = 0; i < 16; i++){
                        Serial.print("0x");
                        Serial.print(data[i], 16);
                        Serial.print(' ');
                    }
                    Serial.println("");

                    if(data[0] == num){
                        Serial.println("Read matches write!");
                    } else {
                        Serial.println("Odd, read does not match write.");
                    }

                    // Wait a bit before reading the card again
                    delay(1000);
                } else {
                    Serial.println("Ooops ... unable to read the requested block. Try another key?");
                }
            } else {
                Serial.println("Ooops ... unable to write to the requested block. Try another key?");
            }
        } else {
            Serial.println("Ooops ... authentication failed: Try another key?");
        }
        // }
        // if (uidLength == 7) {
        //     // We probably have a Mifare Ultralight card ...
        //     Serial.println("Seems to be a Mifare Ultralight tag (7 byte UID)");

        //     // Try to read the first general-purpose user page (#4)
        //     Serial.println("Reading page 4");
        //     uint8_t data[32];
        //     success = nfc.mifareultralight_ReadPage (4, data);
        //     if (success) {
        //         // Data seems to have been read ... spit it out
        //         nfc.PrintHexChar(data, 4);
        //         Serial.println("");

        //         // Wait a bit before reading the card again
        //         delay(1000);
        //     } else {
        //         Serial.println("Ooops ... unable to read the requested page!?");
        //     }
        // }
    }
    *success = _success;
}
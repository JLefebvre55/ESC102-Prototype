#ifndef HELPER_H
#define HELPER_H

// SD/Audio
#include <WaveHC.h>
#include <WaveUtil.h>

// NFC
#include <Wire.h>
#include <Adafruit_PN532.h>

// Define macros to put messages in flash memory
#define fail(msg) fail_P(PSTR(msg))
#define warn(msg) warn_P(PSTR(msg))
#define debug(msg) debug_P(PSTR(msg))

// Pin number macros
#define PN532_IRQ   (2)
#define PN532_RESET (9)  // Not connected by default

// Do debug messages?
#define DEBUG true  // Do debug messages? Set to false for production

extern SdReader card;    // SD card info
extern FatVolume vol;    // Partition info
extern FatReader root;   // Root directory
extern WaveHC wave;      // Audio object
extern Adafruit_PN532 nfc; //NFC reader/writer API
// Timestamp
extern uint32_t t0;
#define TIMESTR String((millis()-t0)/1000.0, 2)+"s"

// Functions
void fail_P(const char* str);
void warn_P(const char* str);
void debug_P(const char* str);
void sdErrorCheck(void);
void post(void);
void findFAT(void);
void playFile(dir_t file);
void printDir(FatReader &dir);

#endif
#include "WAVRegistry.h"

void namecpy(char * dest, dir_t dir){
    uint8_t c = 0;
    for (uint8_t i = 0; i < 11; i++) {
        if (dir.name[i] == ' '){
            continue;
        }
        if (i == 8) {
            dest[c] = '.';
            c++;
        }
        dest[c] = dir.name[i];
        c++;
    }
    for(uint8_t i = c; i < 11; i++){
        dest[c] = '\0';
    }
}

void append(WAVRegistry* tail, dir_t dirBuf){
    // Add to registry
    WAVRegistry* newtail = (WAVRegistry*)malloc(sizeof(WAVRegistry));
    // Make new tail point to head
    newtail->next = tail->next;
    // Old tail point to new tail, etc.
    tail->next = newtail;
    newtail->size = tail->size;
    namecpy(newtail->name, dirBuf);
    // increment length
    *(tail->size) = *(tail->size)+1;
    Serial.print("Appending ");
    Serial.println(newtail->name);
    Serial.print("New size: ");
    Serial.println(*(tail->size));
}

void generateRegistry(WAVRegistry** registry){
    // Allocate registry space (+size)
    *registry = (WAVRegistry*)malloc(sizeof(WAVRegistry));
    (*registry)->size = (uint8_t*)malloc(sizeof(uint8_t));
    *((*registry)->size) = 0;

    WAVRegistry* tail = *registry;
    tail->next = tail;

    // Reusable file pointer
    FatReader file;
    dir_t dirBuf;
    uint8_t dirLevel;
    // Read every file in the directory one at a time
    while (root.readDir(dirBuf) > 0) {
        // Skip if not a .WAV file
        if (DIR_IS_SUBDIR(dirBuf) != 0 && strncmp_P((char *)&dirBuf.name[8], PSTR("WAV"), 3) != 0) {
            printEntryName(dirBuf);
            PgmPrintln(" not a WAV");
            continue;
        }
        if (!file.open(vol, dirBuf)) {  // open the file in the directory
            warn("Failed to open file '"); // something went wrong
            printEntryName(dirBuf);
            PgmPrint("'");
            continue;   // Move on to next file
        }

        // if (file.isDir()) {                   // check if we opened a new directory
        //     putstring("Subdir: ");
        //     printEntryName(dirBuf);
        //     Serial.println();
        //     dirLevel += 2;                      // add more spaces
        //     // play files in subdirectory
        //     play(file);                         // recursive!
        //     dirLevel -= 2;    
        // }
        // else {
        if (!wave.create(file)) {            // Check WAV
            warn("File '");
            printEntryName(dirBuf);
            warn(" is not a valid WAV");     // ok skip it
            continue;
        }

        debug("File found: ");
        printEntryName(dirBuf);
        Serial.println();

        // First entry: just copy name and increment
        if(*(tail->size) == 0){
            namecpy(tail->name, dirBuf);
            // increment length
            *(tail->size) = *(tail->size)+1;
        } else {
            append(tail, dirBuf);
            tail = tail->next;
        }
    }
}
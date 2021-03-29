#include "WAVRegistry.h"

WAVRegistry* append(WAVRegistry* tail, char* path, int id){
    // Add to registry
    WAVRegistry* newtail = (WAVRegistry*)malloc(sizeof(WAVRegistry));
    // Make new tail point to head
    newtail->next = tail->next;
    // Old tail point to new tail, etc.
    tail->next = newtail;
    newtail->size = tail->size;
    strcpy(newtail->name, path);
    // increment length
    *(tail->size) = *(tail->size)+1;
    newtail->id = id;
    return newtail;
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
    FatReader regfile;

    // Open registry file
    if(!regfile.open(root, REGPATH)){
        fail("Failed to find registry file!\n");
    }

    // Set up registry text
    char regtxt[REGTXTLEN];
    memcpy(regtxt, '\0', REGTXTLEN);
    regfile.read(regtxt, REGTXTLEN);

    #if DEBUG
    debug("Registry file contents:\n");
    Serial.println(regtxt);
    #endif

    char* start = regtxt;
    // Step through the registry file
    while (char* end = strchr(start, '\n')) {
        // Find the divisor
        char* div = strchr(start, ' ');
        if(div == NULL){
            warn("Failed to read a registry entry, skipping...\n");
            continue;
        }

        // Insert null character at divider, end
        *div = '\0';
        *end = '\0';

        // Check file extension
        if(strncmp_P(div-3, PSTR("WAV"), 3) != 0){
            warn("Invalid entry: '");
            Serial.print(start);
            PgmPrintln("', skipping...");
            continue;
        }

        // Get ID
        int id = atoi(div+1);

        #if DEBUG
        debug("Entry ");
        Serial.print(id);
        PgmPrintln(" valid");
        #endif

        // Test open file
        if (!file.open(root, start)) {
            warn("Failed to open file '");
            Serial.print(start);
            PgmPrint("', skipping...");
            continue;
        }
        #if DEBUG
        debug("Entry ");
        Serial.print(id);
        PgmPrintln(" opened");
        #endif

        // Test create wave
        if (!wave.create(file)) {
            warn("Failed to read '");
            Serial.println(start);
            PgmPrint("', not a valid WAV, skipping...");
            continue;
        }
        #if DEBUG
        debug("Entry ");
        Serial.print(id);
        PgmPrintln(" decoded");
        #endif

        // First entry: just copy name and increment
        if(*(tail->size) == 0){
            strcpy(tail->name, start);
            tail->id = id;
            *(tail->size) = 1;
        } else {
            tail = append(tail, start, id);
        }
        #if DEBUG
        debug("Entry ");
        Serial.print(id);
        PgmPrint(" appended, new length: ");
        Serial.println(*(tail->size));
        #endif
        start = end+1;
    }
}
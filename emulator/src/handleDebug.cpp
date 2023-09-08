#include "../lib/handleDebug.h"
#include <HardwareSerial.h>
#include <EEPROM.h>
#include "../lib/flash.h"
#include "../lib/chip.h"

void dumpEEPROM(){
    for(int i = 0; i < DATA_SIZE * REDUNDANCY + 1; i++){
        Serial.print(EEPROM[i], HEX);
        Serial.print(" ");
    }
}
void dumpChip(){
    uint8_t* chipData;
    getChipData(&chipData);
    for(int i = 0; i < CHIP_ROW_LENGTH*12; i++){
        Serial.print(chipData[i], HEX);
        if((i+1) % 4 == 0){
            Serial.println("");
        }
        else{
            Serial.print(" ");
        }
    }
}

void setupEEPROM(){
    uint8_t initialData[9] = {
        0x00,                   // Data pointer
        0x12, 0x34, 0x56, 0x78, // Serial
        0x1E, 0xFF, 0xFF, 0xFF, // Counter
    };
    Flash::saveToEEPROM(0, initialData, DATA_SIZE + 1);
}

void handleDebugCommand(){
    String input = Serial.readStringUntil("\n");
    if(input == "dumpEEPROM\n"){
        dumpEEPROM();
    }
    if(input == "dumpChip\n"){
        dumpChip();
    }
    if(input == "setupEEPROM\n"){
        setupEEPROM();
    }
    if(input == "newChip\n"){
        newChip();
    }
    Serial.write("\n");
}
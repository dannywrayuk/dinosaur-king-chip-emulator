#include "../lib/chip.h"
#include <HardwareSerial.h>
#include "../lib/flash.h"

uint8_t chipData[CHIP_CAPACITY] = {
    0x12, 0x34, 0x56, 0x78, // Serial No.0
    0x31, 0x00, 0x86, 0x07, // Serial No.1
    0x00, 0x00, 0x00, 0x00, // Key
    0x05, 0x40, 0x00, 0xAA, // Extend  Extend  Access  Mode
    0x23, 0xFF, 0xFF, 0xFF, // Counter4  Counter3  Counter2  Counter1
    0x00, 0x00, 0x00, 0x00, // User Data
    0x00, 0x00, 0x00, 0x00, // User Data
    0x00, 0x00, 0x00, 0x00, // User Data
    0x00, 0x00, 0x00, 0x00, // User Data
    0x00, 0x00, 0x00, 0x00, // User Data
    0x00, 0x00, 0x00, 0x00, // User Data
    0x23, 0xFF, 0xFF, 0xFF, // User Data
    0x00, 0x00, 0x00, 0x00, // User Data
    0x00, 0x00, 0x00, 0x00, // User Data
    0x00, 0x00, 0x00, 0x00, // User Data
    0x00, 0x00, 0x00, 0x00, // User Data
    0x00, 0x00, 0x00, 0x00, // User Data
    0x00, 0x00, 0x00, 0x00, // User Data
    0x00, 0x00, 0x00, 0x00, // User Data
    0x00, 0x00, 0x00, 0x00, // User Data
    0x00, 0x00, 0x00, 0x00, // User Data
    0x00, 0x00, 0x00, 0x00, // User Data
    0x00, 0x00, 0x00, 0x00, // User Data
    0x00, 0x00, 0x00, 0x00, // User Data
    0x00, 0x00, 0x00, 0x00, // User Data
    0x00, 0x00, 0x00, 0x00, // User Data
    0x00, 0x00, 0x00, 0x00, // User Data
    0x00, 0x00, 0x00, 0x00, // User Data
    0x00, 0x00, 0x00, 0x00, // User Data
    0x00, 0x00, 0x00, 0x00, // User Data
    0x00, 0x00, 0x00, 0x00, // User Data
    0x00, 0x00, 0x00, 0x00, // User Data
};

uint8_t getChipData(uint8_t** dataStart){
    (*dataStart) = &chipData[0];
    return CHIP_CAPACITY;
}

uint8_t getSerial0(uint8_t** dataStart){
    (*dataStart) = &chipData[0];
    return CHIP_ROW_LENGTH;
}

void newChip(){
    Serial.println("Chip counter has reached zero. Generating new chip.");

    // Reset counter to 800
    chipData[CHIP_ROW_LENGTH * 4]     = 0x23;
    chipData[CHIP_ROW_LENGTH * 4 + 1] = 0xFF;
    chipData[CHIP_ROW_LENGTH * 4 + 2] = 0xFF;
    chipData[CHIP_ROW_LENGTH * 4 + 3] = 0xFF;

    // Bump the serial number by 1
    if(chipData[0] == 0xFF){
        if(chipData[1] == 0xFF){
            if(chipData[2] == 0xFF){
               chipData[3] += 1;
            }
            chipData[2] += 1;
        }
        chipData[1] += 1;
    }
    chipData[0] += 1;

    Flash::moveData();
}

uint8_t decrementCounter(uint8_t counterOffset){
    uint8_t id = CHIP_ROW_LENGTH * 4 + counterOffset;
    uint8_t currentValue = chipData[id];
    if(currentValue > 0){
        chipData[id] = currentValue - 1;
    }
    if(chipData[CHIP_ROW_LENGTH * 4]
        + chipData[CHIP_ROW_LENGTH * 4 + 1]
        + chipData[CHIP_ROW_LENGTH * 4 + 2]
        + chipData[CHIP_ROW_LENGTH * 4 + 3] < 2){
        newChip();
    }
    else{
       Flash::updateData();
    }
    return currentValue;
}
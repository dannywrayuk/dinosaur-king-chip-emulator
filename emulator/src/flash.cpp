#include "../lib/flash.h"
#include <EEPROM.h>
#include <HardwareSerial.h>
#include "../lib/chip.h"

void saveToRam(uint8_t startAddress, uint8_t* target, uint8_t length){
    uint8_t value;
    for(int i = 0; i < length; i++){
        EEPROM.get(startAddress + i, value);
        *(target + i) = value;
    }
}

void Flash::saveToEEPROM(uint8_t startAddress, uint8_t* source, uint8_t length){
    for(int i = 0; i < length; i++){
        EEPROM.update(startAddress + i, *(source + i));
    }
}

void save(uint8_t pointer){
    uint8_t* chipData;
    getChipData(&chipData);

    Flash::saveToEEPROM(1 + pointer * DATA_SIZE, chipData, CHIP_ROW_LENGTH); // Save serial
    Flash::saveToEEPROM(1 + pointer * DATA_SIZE + CHIP_ROW_LENGTH, chipData + CHIP_ROW_LENGTH * 4, CHIP_ROW_LENGTH); // Save counters
}


void Flash::initialise(){
    uint8_t dataPointer = 0;
    uint8_t* chipData;
    EEPROM.get(DATA_POINTER, dataPointer);
    getChipData(&chipData);
    saveToRam(1 + dataPointer * DATA_SIZE, chipData, CHIP_ROW_LENGTH); // Copy serial
    saveToRam(1 + dataPointer * DATA_SIZE + CHIP_ROW_LENGTH, chipData + CHIP_ROW_LENGTH * 4, CHIP_ROW_LENGTH); // Copy counters
}

void Flash::updateData(){
    uint8_t dataPointer = 0;
    EEPROM.get(DATA_POINTER, dataPointer);
    save(dataPointer);
}

void Flash::moveData(){
    uint8_t dataPointer = 0;
    EEPROM.get(DATA_POINTER, dataPointer);

    uint8_t newPointer = (++dataPointer) % REDUNDANCY;
    Serial.println(newPointer);
    EEPROM.update(DATA_POINTER, newPointer);

    save(newPointer);
}
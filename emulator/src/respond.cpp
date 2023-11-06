#include "../lib/respond.h"
#include <HardwareSerial.h>
#include "../lib/SerialController.h"
#include "../lib/chip.h"

#define NO_NEXT nullptr

#define EXPECT(value, next)    \
                        [](uint8_t input) -> void** {               \
                            if(input == value){                     \
                                return next;                        \
                            }                                       \
                            return NO_NEXT;                         \
                        }

#define RESPOND(value, response, next)   \
                        [](uint8_t input) -> void** {               \
                            if(input == value){                     \
                                SerialController::write(response);  \
                                return next;                        \
                            }                                       \
                            return NO_NEXT;                         \
                        }

#define ANY(next)   \
                        [](uint8_t input) -> void** {               \
                            return next;                            \
                        }

#define ANY_RESPOND(response, next)   \
                        [](uint8_t input) -> void** {               \
                            SerialController::write(response);      \
                            return next;                            \
                        }


void* end[3] = {
    EXPECT(0x5E, &end[1]),
    EXPECT(0x80, &end[2]),
    RESPOND(0x00, 0x0A, NO_NEXT),
};

void* maskedData[4] = {
    EXPECT(0x00, &maskedData[1]),
    EXPECT(0x00, &maskedData[2]),
    RESPOND(0x00, 0x0A, &maskedData[3]),
    [](uint8_t input) -> void** {
        if(input == 0x0E){
            SerialController::write(0x0A);
            uint8_t* response;
            getChipData(&response);
            SerialController::write(response, 8);
            SerialController::write(new uint8_t[120]{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAA}, 120);
            return &end[0];
        }
        return NO_NEXT;
    },
};

void* decrement[1] = {
    [](uint8_t input) -> void** {
        SerialController::write(0x0A);
        if(input == 0xC0){
            SerialController::write(decrementCounter(0));
            return &end[0];
        }
        if(input == 0x30){
            SerialController::write(decrementCounter(1));
            return &end[0];
        }
         if(input == 0x0C){
            SerialController::write(decrementCounter(2));
            return &end[0];
        }
        if(input == 0x03){
            SerialController::write(decrementCounter(3));
            return &end[0];
        }
        return NO_NEXT;
    },
};

uint8_t rowCounter = 0;
uint8_t byteCounter = 0;
uint8_t byteList[4] = {};
bool shouldUpdateBytes = false;

void updateBytes(){
    uint8_t* chipData;
    getChipData(&chipData);
    memcpy(chipData+CHIP_ROW_LENGTH*rowCounter, byteList, 4);
}

void* updateRows[2] = {
    [](uint8_t input) -> void** {
        byteList[byteCounter] = input;
        if(byteCounter >= 3){
            if(shouldUpdateBytes) updateBytes();
            SerialController::write(0x0A);
            return &updateRows[1];
        }
        byteCounter++;
        return &updateRows[0];
    },
    [](uint8_t input) -> void** {
        byteCounter = 0;
        shouldUpdateBytes = false;
        if(input == 0x0E){
            SerialController::write(0x0A);
            uint8_t* response;
            int len = getChipData(&response);
            SerialController::write(response, len);
            return &end[0];
        }
        if(input == 0xEE || input == 0x8E){
            rowCounter++;
            return &updateRows[0];
        }
        if(input == 0xCE){
            rowCounter++;
            shouldUpdateBytes = true;
            return &updateRows[0];
        }
        return NO_NEXT;
    },
};

void* fullData[4] = {
    ANY(&fullData[1]),
    ANY(&fullData[2]),
    ANY_RESPOND(0x0A, &fullData[3]),
    [](uint8_t input) -> void** {
        if(input == 0x0E){
            SerialController::write(0x0A);
            uint8_t* response;
            int len = getChipData(&response);
            SerialController::write(response, len);
            return &end[0];
        }
        if(input == 0x6E){
            return &decrement[0];
        }
        if(input == 0xEE){
            rowCounter = 0;
            byteCounter = 0;
            shouldUpdateBytes = false;
            return &updateRows[0];
        }
        return NO_NEXT;
    },
};

void* serialEcho[6] = {
    EXPECT(0x2E, &serialEcho[1]),
    ANY(&serialEcho[2]),
    ANY(&serialEcho[3]),
    ANY(&serialEcho[4]),
    ANY(&serialEcho[5]),
    [](uint8_t input) -> void** {
        if(input == 0x00){
            return &maskedData[0];
        }
        return &fullData[0];
    },
};

void* readSerial[4] = {
    EXPECT(0x5E, &readSerial[1]),
    EXPECT(0x00, &readSerial[2]),
    [](uint8_t input) -> void** {
        if(input >= 0x20 && input <= 0x2F){
            SerialController::write(0x0A);
            return &readSerial[3];
        }
        return NO_NEXT;
    },
    [](uint8_t input) -> void** {
        if(input == 0x4E){
            SerialController::write(0x0A);
            uint8_t* response;
            int len = getSerial0(&response);
            SerialController::write(response, len);
            return &serialEcho[0];
        }
        return NO_NEXT;
    },
};

void* initialise[4] = {
    EXPECT(0x5E, &initialise[1]),
    EXPECT(0x00, &initialise[2]),
    [](uint8_t input) -> void** {
        if(input >= 0x20 && input <= 0x2F){
            SerialController::write(0x6A);
            return &initialise[3];
        }
        return NO_NEXT;
    },
    RESPOND(0x4E, 0xCA, &readSerial[0]),
};

void** current = &initialise[0];

void respond(uint8_t inputByte){
    void** (*responseFn)(uint8_t);
    responseFn = *current;
    void** next = responseFn(inputByte);
    if(next != NO_NEXT){
        current = next;
    }
    else {
        current = &initialise[0];
    }
}
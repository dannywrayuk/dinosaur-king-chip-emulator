#include "../lib/SerialController.h"
#include <HardwareSerial.h>
#include "../lib/utils.h"


void SerialController::initialise(){
    Serial1.begin(19200, SERIAL_8E1);
    return;
};

int SerialController::available(){
    return Serial1.available();
};

uint8_t SerialController::read(){
    // String input = Serial.readStringUntil('\n');
    // input.trim();
    // Serial.println(input);
    // return strtol(input.c_str(), NULL, 16);
    uint8_t read = Serial1.read();
    Serial.println(read, HEX);
    return read;
}

void SerialController::write(uint8_t* bytes, int length){
    Serial1.write(bytes, length);
    printBytes(bytes, length);
}
void SerialController::write(uint8_t byte){
    Serial1.write(byte);
    Serial.print("\t");
    Serial.println(byte, HEX);
}

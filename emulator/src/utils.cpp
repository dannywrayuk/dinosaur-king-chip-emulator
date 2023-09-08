#include "../lib/utils.h"
#include <HardwareSerial.h>

void printBytes(uint8_t* responseBytes, int responseLength){
    for(int i = 0; i< responseLength; i++){
        Serial.print("\t");
        Serial.println(responseBytes[i], HEX);
    }
}
#include "./lib/SerialController.h"
#include "./lib/respond.h"
#include "./lib/flash.h"
#include "./lib/handleDebug.h"

void setup() {
  Serial.begin(115200); // Communication with the PC must be fast enough not to interfere with the serial communication.
  Serial.println("Started listening..");
  SerialController::initialise();
  Flash::initialise();
}

void loop() {
  if(SerialController::available()){
    uint8_t readByte = SerialController::read();
    respond(readByte);
  }
  if(Serial.available()){
    handleDebugCommand();
  }
}
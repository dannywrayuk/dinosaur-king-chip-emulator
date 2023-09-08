#include <stdint.h>

class SerialController {
    public:
        static void initialise();
        static int available();
        static uint8_t read();
        static void write(uint8_t* bytes, int length);
        static void SerialController::write(uint8_t byte);
};

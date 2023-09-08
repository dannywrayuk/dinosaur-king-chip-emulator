#include <stdint.h>
#define REDUNDANCY 3
#define DATA_POINTER 0
#define DATA_SIZE 8

class Flash {
    public:
        static void saveToEEPROM(uint8_t startAddress, uint8_t* source, uint8_t length);
        static void initialise();
        static void moveData();
        static void updateData();

};
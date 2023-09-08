#include <stdint.h>

#define CHIP_ROW_LENGTH 4
#define CHIP_CAPACITY 32 * CHIP_ROW_LENGTH
#define CHIP_COUNTER_OFFSET 4 * CHIP_ROW_LENGTH

uint8_t getChipData(uint8_t** dataStart);

uint8_t getSerial0(uint8_t** dataStart);

uint8_t decrementCounter(uint8_t counterOffset);

void newChip();
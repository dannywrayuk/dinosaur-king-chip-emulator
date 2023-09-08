#include <stdint.h>

typedef struct dataInsert {
    uint8_t index;
    uint8_t dataId;
}dataInsert;

typedef struct byteNode {
    uint8_t value;

    uint8_t responseLength;
    uint8_t* responses;

    uint8_t chipDataLength;
    dataInsert* chipData;

    uint8_t nextByteLength;
    byteNode* nextBytes;

    char* milestone;
}byteNode;

class ByteNode {
    public:
        byteNode node;
        ByteNode();
        ByteNode setValue(uint8_t value);
        ByteNode addResponses(uint8_t count, uint8_t* responses);
        ByteNode addChipData(uint8_t count, dataInsert* chipData);
        ByteNode addNextByte(uint8_t count, byteNode* chipData);
        ByteNode addMilestone(char* milestone);
};
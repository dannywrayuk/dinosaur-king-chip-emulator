#include "../lib/byteNode.h"

ByteNode::ByteNode(){
    node.value = 0x00;

    node.responseLength = 0;
    node. responses = nullptr;

    node.chipDataLength = 0;
    node.chipData = nullptr;

    node.nextByteLength = 0;
    node.nextBytes = nullptr;

    node.milestone = nullptr;
};

ByteNode ByteNode::setValue(uint8_t value){
    node.value = value;
    return *this;
};

ByteNode ByteNode::addResponses(uint8_t count, uint8_t* responses){
    node.responseLength = count;
    node.responses = responses;
    return *this;
};

ByteNode ByteNode::addChipData(uint8_t count, dataInsert* chipData){
    node.chipDataLength = count;
    node.chipData = chipData;
    return *this;
};

ByteNode ByteNode::addNextByte(uint8_t count, byteNode* nextBytes){
    node.nextByteLength = count;
    node.nextBytes = nextBytes;
    return *this;
};

ByteNode ByteNode::addMilestone(char* milestone){
    node.milestone = milestone;
    return *this;
};
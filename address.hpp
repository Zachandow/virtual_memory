
#pragma once
using BYTE = uint8_t;
using SBYTE = int8_t;

class LogicalAddress {
private:
    uint16_t address; 

public:

    LogicalAddress(int loc) {
        this->address = static_cast<decltype(this->address)>(loc);
    }

    BYTE getPage() {
        return (0xFF00 & this->address) >> 8;
    }

    BYTE getOffset() {
        return (0xFF & this->address);
    }

};

#include "signverify.h"
#include <vector>
#include <string>
#include <bitset>

std::string toBinaryString(const std::string& message);

std::vector<uint8_t> Sign(std::string message, std::vector<uint8_t> sk) {
    std::vector<uint8_t> signature;

    // Convert message to binary string
    std::string binmessage = toBinaryString(message);

    // Reverse the key's length
    size_t k = (sk.size() * 8) / (message.size() * 2);

    // Core logic
    for(size_t i = 0; i < binmessage.size(); i++) {
        if(binmessage[i] == '0') {
            signature.insert(signature.end(), sk.begin() + i * k/8, sk.begin() + (i + 1) * k/8);
        } else if(binmessage[i] == '1') {
            signature.insert(signature.end(), sk.begin() + (i + message.size()) * k/8, sk.begin() + (i + message.size() + 1) * k/8);
        }
    }

    return signature;
}

bool Verify(std::string message, std::vector<uint8_t> sig, std::vector<uint8_t> pk){
    return true;
}

std::string toBinaryString(const std::string& message) {
    std::string binaryString;
    for (uint8_t byte : message) {
        std::bitset<8> bits(byte); // Convert each byte to a bitset of 8 bits
        binaryString += bits.to_string(); // Append as a binary string
    }
    return binaryString;
}
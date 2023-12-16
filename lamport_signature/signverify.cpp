#include "signverify.h"
#include "keygen.h"
#include <vector>
#include <string>

std::vector<uint8_t> getPKPart(const std::string &message, const std::vector<uint8_t> &pk, size_t i, size_t k);

std::vector<uint8_t> Sign(const std::string &message, const std::vector<uint8_t> &sk) {
    std::vector<uint8_t> signature;

    // Reverse the key's length
    size_t k = 8 * sk.size() / (message.size() * 2);

    // Core logic
    // Sign each bit of the binary message
    for(size_t i = 0; i < message.size(); i++) {
        if(message[i] == '0') {
            signature.insert(signature.end(), sk.begin() + i * k / 8, sk.begin() + (i + 1) * k / 8);
        } else if(message[i] == '1') {
            signature.insert(signature.end(), sk.begin() + (i + message.size()) * k / 8, sk.begin() + (i + message.size() + 1) * k / 8);
        }
    }

    return signature;
}

bool Verify(const std::string &message, const std::vector<uint8_t> &sig, const std::vector<uint8_t> &pk){
    // Reverse the key's length
    size_t k = 8 * pk.size() / (message.size() * 2);

    // Check each bit of the binary message
    for (size_t i = 0; i < message.size(); ++i) {
        // Get the corresponding hash from the public key
        std::vector<uint8_t> pk_part = getPKPart(message, pk, i, k);

        // Hash the corresponding part of the signature
        std::vector<uint8_t> sig_part(sig.begin() + i * k / 8, sig.begin() + (i + 1) * k / 8);
        std::vector<uint8_t> hash_sig_part = calculateSHA256(sig_part);

        // Compare the hash of the signature part with the corresponding public key part
        if (hash_sig_part != pk_part) {
            return false;
        }
    }

    return true;
}

// Get the corresponding hash from the public key
std::vector<uint8_t> getPKPart(const std::string &message, const std::vector<uint8_t> &pk, size_t i, size_t k) {
    if(message[i] == '0') {
        std::vector<uint8_t> pk_part(pk.begin() + i * k / 8, pk.begin() + (i + 1) * k / 8);
        return pk_part;
    } else if(message[i] == '1') {
        std::vector<uint8_t> pk_part(pk.begin() + (i + message.size()) * k / 8, pk.begin() + (i + message.size() + 1) * k / 8);
        return pk_part;
    }
}
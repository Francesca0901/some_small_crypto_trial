#ifndef KEYGEN_H
#define KEYGEN_H

#include <vector>
#include <cstdint>

struct KeyPair {
    std::vector<uint8_t> publicKey;
    std::vector<uint8_t> privateKey;

    KeyPair(const std::vector<uint8_t>& pk, const std::vector<uint8_t>& sk)
        : publicKey(pk), privateKey(sk) {}
};

// n: bitlength of the message k: a security parameter
KeyPair KeyGen(std::size_t n, std::size_t k);

std::vector<uint8_t> calculateSHA256(const std::vector<uint8_t>& data);

#endif // KEYGEN_H
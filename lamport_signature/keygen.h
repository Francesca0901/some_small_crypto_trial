#ifndef KEYGEN_H
#define KEYGEN_H

#include <vector>

struct KeyPair {
    std::vector<uint8_t> publicKey;
    std::vector<uint8_t> privateKey;
};

// n: bitlength of the message k: a security parameter
KeyPair KeyGen(size_t n, size_t k);

#endif // KEYGEN_H
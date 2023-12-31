#include "keygen.h"
#include <vector>
#include <random>
#include <bitset>
#include <openssl/sha.h>

std::vector<uint8_t> generateRandomKbits(size_t k) ;
std::vector<uint8_t> calculateSHA256(const std::vector<uint8_t>& data);

// The way to store the key: bit0_0, bit1_0, bit2_0, ..., bitn_0,
//                           bit0_1, bit1)1, bit2_1, ..., bitn_1
KeyPair KeyGen(std::size_t n, std::size_t k){
    // Private key generation
    // Generate 2n ramdon values with k bits each
    std::vector<uint8_t> sk;
    for(size_t i = 0; i < n * 2; i++) {
        std::vector<uint8_t> randomBits = generateRandomKbits(k);
        sk.insert(sk.end(), randomBits.begin(), randomBits.end());
    }
    
    // Public key generation
    std::vector<uint8_t> pk;
    for(size_t i = 0; i < n * 2; i++) {
        std::vector<uint8_t> kbitValue(sk.begin() + i * k/8, sk.begin() + (i + 1) * k/8);
        std::vector<uint8_t> hash_skij = calculateSHA256(kbitValue);
        pk.insert(pk.end(), hash_skij.begin(), hash_skij.end());
    }

    KeyPair keyPair(pk, sk);
    return keyPair;
}

std::vector<uint8_t> generateRandomKbits(size_t k) {
    // K can be expected to be a multiple of 8
    size_t num_bytes = k / 8; // 8 bytes per block

    std::vector<uint8_t> random_bits(num_bytes, 0);

    // Random number generation
    std::random_device rd;
    std::mt19937_64 gen(rd()); 
    std::uniform_int_distribution<uint8_t> dis;

    for (size_t i = 0; i < num_bytes; ++i) {
        random_bits[i] = dis(gen);  // Directly assign a random byte
    }

    return random_bits;
}


// Borrowed from https://stackoverflow.com/questions/2262386/generate-sha256-with-openssl-and-c
std::vector<uint8_t> calculateSHA256(const std::vector<uint8_t>& data) {
    uint8_t hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256; 
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, data.data(), data.size());
    SHA256_Final(hash, &sha256);
    
    return std::vector<uint8_t>(hash, hash + SHA256_DIGEST_LENGTH);
}
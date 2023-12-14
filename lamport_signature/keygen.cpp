#include "keygen.h"
#include <vector>
#include <random>
#include <bitset>
#include <openssl/sha.h>

std::vector<uint8_t> generateRandomKbits(size_t k) ;
std::vector<uint8_t> calculateSHA256(const std::vector<uint8_t>& data);

KeyPair KeyGen(std::size_t n, std::size_t k){
    // Private key generation
    // Generate 2n ramdon values with k bits, C++ doesn't seems have built-in function to generate longer than 64 bits
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
    // K can be expected to be divided by 64
    size_t num_blocks = k / 64; 
    size_t num_bytes = num_blocks * 8; // 8 bytes per block

    std::vector<uint8_t> random_bits(num_bytes, 0);

    // Random number generation
    std::random_device rd;
    std::mt19937_64 gen(rd()); // Use mt19937_64 for generating 64-bit random numbers
    std::uniform_int_distribution<uint64_t> dis;

    for(size_t i = 0; i < num_blocks; i++) {
        uint64_t random_block = dis(gen);
        for(size_t j = 0; j < 8; j++) {
            random_bits[i * 8 + j] = (random_block >> (j * 8)) & 0xFF;
        }
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
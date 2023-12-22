#include <iostream>
#include <vector>
#include <random>

#include "keygen.h"
#include "signverify.h"


std::string generateRandomBinaryString(std::size_t length) 
{ 
    std::random_device rd; 
    std::mt19937 gen(rd()); 
    std::uniform_int_distribution<> dis(0, 1); // Only two possibilities: 0 or 1

    std::string random_binary_string;
    random_binary_string.reserve(length); // Allocate the space in advance

    for (std::size_t i = 0; i < length; ++i)
    {
        // Add either '0' or '1' to the string
        random_binary_string.push_back(dis(gen) == 0 ? '0' : '1');
    }

    return random_binary_string;
}


bool run(size_t l, size_t n, size_t k) {
    for (size_t i = 0; i < l; ++i) {
        // 1. Generate keys for each round - finished
        KeyPair keys = KeyGen(n, k);

        // 2. Generate a random message of length n - finished
        std::string message = generateRandomBinaryString(n);

        // 3. Sign the message
        std::vector<uint8_t> signature = Sign(message, keys.privateKey);

        // 4. Verify the signature
        bool isValid = Verify(message, signature, keys.publicKey);

        // 5. Print the result
        std::cout << "i = " << i << " -- verify = " << (isValid ? "true" : "false") << std::endl;

        if (!isValid) {
            // If any verification fails, return false
            return false;
        }
    }
    // If all verifications pass, return true
    return true;
}


int main() {
    // Let's run 10 rounds of signing and verification, make msg length 1000 bits, and security parameter 256
    bool verification = run(10, 1000, 256); 

    std::cout << "All signatures verified: " << (verification ? "Yes" : "No") << std::endl;
    return 0;
}


// an example to explain

// Keygen: n = 2, k = 4
// sk = bit0: {0:A, 1:B} 
//      bit1: {0:C, 1:D} 
// pk = bit0: {H(A), H(B)} = {0:E, 1:F}
//      bit1: {H(C), H(D)} = {0:G, 1:H}

// Sign: m = 01
// sig = {A, D}

// Verify: m = 01, sig = {A, D}, pk = {E, F, G, H}
// verify = {E, H}
// H(A) == E, H(D) == H, pass
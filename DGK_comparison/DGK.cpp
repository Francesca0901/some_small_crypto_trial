#include <iostream> 
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>

// Bit representation: integer 𝑥, a bit-length 𝑙 and outputs an array containing the bit-representation of 𝑥
std::vector<uint8_t> generate(uint64_t x, size_t l) {

    if(x > 0) { // if x == 0, log2(0) is undefined
        // find the bit length of x
        size_t bit_length = std::log2(x) + 1;
        if (bit_length > l) {
            std::cout << "Error: l is smaller than x's size" << std::endl;
            return {};
        }
    }

    std::vector<uint8_t> result(l, 0);
    for(size_t i = 0; i < l; i++) {
        result[i] = (x >> i) & 0x1;
    }
    return result;
}

// Takes an array and returns a random permutation of that array
std::vector<int> permute(const std::vector<int> &z) {
    std::vector<int> permuted(z.begin(), z.end());
    
    auto rd = std::random_device {}; 
    auto rng = std::default_random_engine { rd() };

    // Shuffle the vector
    std::shuffle(std::begin(permuted), std::end(permuted), rng);

    return permuted;
}

// Compute vector z
std::vector<int> cal_z(const std::vector<uint8_t> &x, const std::vector<uint8_t> &y, size_t l) {
    std::vector<int> z(l, 0); // int or not?

    for (size_t i = 0; i < l; i++) {
        int sum = 0;
        // Calculate the summation part of the formula
        for (size_t j = i + 1; j < l; j++) {
            sum += (1 - (x[j] ^ y[j]));
        }
        // Compute z_i using the given formula
        z[i] = 1 + x[i] - y[i] + 3 * sum;
    }

    return z;
}

// Compute vector z and permulate it
std::vector<int> evaluate(const std::vector<uint8_t> &x, const std::vector<uint8_t> &y, size_t l) {
    // Compute vector z
    std::vector<int> z = cal_z(x, y, l);

    // Permute vector z
    std::vector<int> z_permuted = permute(z);

    return z_permuted;
}

// Compute the final result
bool finalize(const std::vector<int> &z_permuted, size_t l) {
    for(size_t i = 0; i < l; i++) {
        if(z_permuted[i] == 0) {
            return true;
        }
    }

    return false;
}

bool compare(uint64_t x, uint64_t y, size_t l) {
    // Generate bit representation of x and y
    std::vector<uint8_t> x_bit = generate(x, l);
    std::vector<uint8_t> y_bit = generate(y, l);

    // If x or y is not successfully generated, return false
    if(x_bit.size() == 0 || y_bit.size() == 0) {
        return false;
    }

    // Compute vector z and permulate it
    std::vector<int> z_permuted = evaluate(x_bit, y_bit, l);

    // Compute the final result
    bool result = finalize(z_permuted, l);

    return result;
}

void timing(int n, size_t l) {
    std::random_device rd; // Seed with a real random value, if available
    std::mt19937_64 rng(rd()); // Use the 64-bit Mersenne Twister 19937 generator
    std::uniform_int_distribution<uint64_t> dist(0, std::numeric_limits<uint64_t>::max());

    for(size_t i = 0; i < n; i++) {
        // generate two 64-bit unsigned integers x and y
        uint64_t x = dist(rng);
        uint64_t y = dist(rng);
        bool result = compare(x, y, l);

        std::cout << "i = " << i 
                  << " -- x = " << x 
                  << " -- y = " << y 
                  << " -- c = " << (result ? "true" : "false") << std::endl;
    }

    return;
}

void test() {
    int x = 6;
    int y = 8;
    std::cout << (compare(x, y, 4) ? "true" : "false") << std::endl;
    return;
}

int main() {
    srand(time(NULL));

    // test();

    // Test 10 times, let length be 64 bits
    timing(10, 64);

    return 0;
}
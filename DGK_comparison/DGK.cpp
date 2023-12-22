#include <iostream> 
#include <vector>
#include <cmath>
#include <random>
#include <cassert>
#include <ctime>
#include <cstdlib>

// Bit representation: integer 𝑥, a bit-length 𝑙 and outputs an array containing the bit-representation of 𝑥
std::vector<uint8_t> generate(uint64_t x, size_t l) {

    if(x > 0) { // if x == 0, log2(0) is undefined
        // find the bit length of x
        size_t bit_length = std::log2(x) + 1;
        if (bit_length > l) {
            std::cout << "Error: l is smaller than x's size. l = " << l << " while x contains " << bit_length << " bits." << std::endl;
            return {};
        }
    }

    std::vector<uint8_t> result(l, 0);
    for(size_t i = 0; i < l; i++) {
        result[l - 1 - i] = (x >> i) & 0x1;
    }
    return result;
}

// A utility function to swap to integers 
void swap (int *a, int *b) 
{ 
    int temp = *a; 
    *a = *b; 
    *b = temp; 
} 

// Fisher–Yates shuffle, O(n) time complexity
std::vector<int> permute(const std::vector<int> &z) {
    std::vector<int> permuted(z.begin(), z.end());
    
   // Initialize random seed
    std::srand(time(NULL));

    for (std::size_t i = permuted.size() - 1; i > 0; i--) 
    { 
        // Pick a random index from 0 to i 
        std::size_t j = rand() % (i + 1); 
 
        // Swap arr[i] with the element at random index 
        swap(&permuted[i], &permuted[j]); 
    } 

    return permuted;
}

// Compute vector z, O(l^2) time complexity
std::vector<int> cal_z(const std::vector<uint8_t> &x, const std::vector<uint8_t> &y, size_t l) {
    std::vector<int> z(l, 0); 

    for (size_t i = 0; i < l; i++) {
        int sum = 0;
        // Calculate the sum
        for (int j = i - 1; j >= 0; j--) {
            sum += (x[j] ^ y[j]);
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

    // Create a bitmask with only lower l bits set to 1
    uint64_t bitmask = (l >= 64) ? std::numeric_limits<uint64_t>::max() : (1ULL << l) - 1;

    for(size_t i = 0; i < n; i++) {
        // generate two 64-bit unsigned integers x and y, and cut them into l bits
        uint64_t x = dist(rng) & bitmask;
        uint64_t y = dist(rng) & bitmask;
        bool result = compare(x, y, l);

        std::cout << "i = " << i 
                  << " -- x = " << x 
                  << " -- y = " << y 
                  << " -- c = " << (result ? "true" : "false") << std::endl;
    }

    return;
}


// --------------------- Tests ---------------------
void testGenerate() {
    std::cout << "Testing generate function" << std::endl;

    assert(generate(0, 4) == (std::vector<uint8_t>{0, 0, 0, 0}));
    assert(generate(15, 4) == (std::vector<uint8_t>{1, 1, 1, 1}));
    assert(generate(1, 8) == (std::vector<uint8_t>{0, 0, 0, 0, 0, 0, 0, 1}));

    std::cout << "generate function passed all tests." << std::endl;
}

void testPermute() {
    std::cout << "Testing permute function" << std::endl;

    // a basic test to check size.
    assert(permute({1, 2, 3, 4, 5}).size() == 5);
    assert(permute({}).empty());

    std::cout << "permute function passed basic tests." << std::endl;
}

void testFinalize() {
    std::cout << "Testing finalize function" << std::endl;

    assert(finalize({0, 1, 2}, 3) == true);
    assert(finalize({1, 2, 3}, 3) == false);

    std::cout << "finalize function passed all tests." << std::endl;
}

void testCompare() {
    std::cout << "Testing compare function" << std::endl;

    assert(compare(2, 3, 64) == true); 
    assert(compare(100000, 100000, 64) == false); 
    assert(compare(987654321, 123456789, 64) == false); 
    assert(compare(123456789, 987654321, 64) == true); 
    assert(compare(123456789, 234567890, 64) == true); 

    std::cout << "compare function passed basic tests." << std::endl;
}

// --------------------- Main ---------------------

int main() {
    srand(time(NULL));

    // Run the tests
    // testGenerate();
    // testPermute();
    // testFinalize();
    // testCompare();

    // Test 10 times, let length 50 bits
    timing(10, 50);

    return 0;
}
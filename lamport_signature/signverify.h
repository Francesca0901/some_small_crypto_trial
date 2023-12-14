#ifndef SIGNVERIFY_H
#define SIGNVERIFY_H

#include <vector>
#include <cstdint>

std::vector<uint8_t> Sign(std::string message, std::vector<uint8_t> sk);

bool Verify(std::string message, std::vector<uint8_t> sig, std::vector<uint8_t> pk);

#endif // SIGNVERIFY_H
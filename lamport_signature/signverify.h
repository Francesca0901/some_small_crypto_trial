#ifndef SIGNVERIFY_H
#define SIGNVERIFY_H

#include <vector>
#include <string>
#include <cstdint>

std::vector<uint8_t> Sign(const std::string &message, const std::vector<uint8_t> &sk);

bool Verify(const std::string &message, const std::vector<uint8_t> &sig, const std::vector<uint8_t> &pk);

#endif // SIGNVERIFY_H
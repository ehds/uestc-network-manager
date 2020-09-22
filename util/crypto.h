#ifndef UESTC_CRYPTO_H_
#define UESTC_CRYPTO_H_

#include <algorithm>
#include <iostream>
#include <iterator>
#include <math.h>
#include <openssl/hmac.h>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>

namespace uestc {

int HmacMD5(const char* key, unsigned int key_length, const char* input,
            unsigned int input_length, unsigned char*& output,
            unsigned int& output_length);
std::string HmacMD5(const std::string& data, const std::string& token);
std::string Base64(const std::string& s);
std::vector<unsigned char> XEncode(const std::string& msg, const std::string& key);
}  // namespace uestc
#endif
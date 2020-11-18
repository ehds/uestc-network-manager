// Copyright (c) 2020 The UESTC-NetworkManager Authors. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Authors: ehds(ds.he@foxmail.com)
#ifndef UESTC_CRYPTO_H_
#define UESTC_CRYPTO_H_
#include <string>
#include <vector>

namespace uestc {

int HmacMD5(const char* key, unsigned int key_length, const char* input,
            unsigned int input_length, unsigned char*& output,
            unsigned int& output_length);
std::string HmacMD5(const std::string& data, const std::string& token);
std::string Base64(const std::vector<unsigned char>& s);
std::vector<unsigned char> XEncode(const std::string& msg,
                                   const std::string& key);
std::string Sha1(const std::string& data);
}  // namespace uestc
#endif
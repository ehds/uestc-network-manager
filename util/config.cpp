// Copyright (c) 2020 The UESTC-NetworkManager Authors. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Authors: ehds(ds.he@foxmail.com)
#include "util/config.h"

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
namespace uestc {
Config::Config(const std::string config_path) : config_path_(config_path) {
  ParseConfig();
}
bool Config::ParseConfig() {
  std::ifstream file(config_path_.data());
  if (!file.is_open()) {
    std::cerr << "Error while opening file " << config_path_ << strerror(errno)
              << std::endl;
    return false;
  }
  auto parse_line = [&](const std::string& line) {
    if (line == "" || line[0] == '#') return;
    size_t pos = line.find('=');
    if (pos == std::string::npos) return;
    // TODO trim string
    std::string option = line.substr(0, pos);
    std::string value = line.substr(pos + 1);
    data_[option] = value;
  };
  std::string line;
  while (file.good()) {
    std::getline(file, line);
    parse_line(line);
  }
  return true;
}
std::string Config::get(const std::string &key){
    if(data_.find(key) != data_.end()){
        return data_[key];
    }
    return "";
}
}  // namespace uestc
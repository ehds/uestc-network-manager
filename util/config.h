#ifndef UESTC_CONFIG_H_
#define UESTC_CONFIG_H_
#include <string>
#include <unordered_map>
namespace uestc {
    const static std::string kConfigPath = "/etc/uestc_networkmanager/user.conf";
class Config {
 public:
  Config(const std::string config_path = kConfigPath);
  
  ~Config() = default;
  Config(const Config&) = delete;
  Config& operator=(const Config&) = delete;
  Config(Config&&) = delete;
  Config& operator=(Config&&) = delete;
  //Return value of the key, "" if key is not exists 
  std::string get(const std::string& key);
  private:
  bool ParseConfig();
  std::string config_path_;
  std::unordered_map<std::string,std::string> data_;
};
}  // namespace uestc

#endif
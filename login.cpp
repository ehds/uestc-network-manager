#define CPPHTTPLIB_ZLIB_SUPPORT
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <chrono>
#include <iostream>
#include <memory>
#include <regex>
#include <time.h>
#include <unordered_map>

#include "util/crypto.h"

#include "httplib.h"
using InfoList = std::vector<std::pair<std::string,std::string> >;

struct UserInfo {
  std::string username;
  std::string password;
  std::string ip;
  std::string acid;
  std::string enc;
  UserInfo(const std::string& username, const std::string& password,
           const std::string& ip, const std::string& acid,
           const std::string& enc)
      : username(username), password(password), ip(ip), acid(acid){};
};

class Client {
 public:
  Client(const std::string& host) : host_(host) {
    client_ =
        std::unique_ptr<httplib::Client>(new httplib::Client(host_.c_str()));
  };
  Client(const Client& client) = delete;
  Client(Client&& client) = delete;
  std::time_t GetTimeStamp() {
    auto tp = std::chrono::time_point_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now());
    std::time_t timestamp = tp.time_since_epoch().count();
    return timestamp;
  }

  bool GetChallenge(UserInfo& user_info, std::string& result) {
    std::string path = "/cgi-bin/get_challenge";
    std::unordered_map<std::string, std::string> params;
    auto timestamp = GetTimeStamp();
    params["callback"] =
        "jQuery11240889396928485537_" + std::to_string(timestamp);
    params["username"] = user_info.username;
    params["_"] = std::to_string(timestamp);
    params["ip"] = user_info.ip;

    std::stringstream ss;
    for (auto p : params) {
      ss << p.first << "=" << p.second << "&";
    }
    path += ("?" + ss.str());
    if (auto res = client_->Get(path.c_str())) {
      if (res->status == 200) {
        std::smatch results;
        if (std::regex_search(res->body, results,
                              std::regex("\"challenge\":\"(.*?)\""))) {
          result = std::move(results[1]);  // the value of challenge
          return true;
        }
      }
    } else {
      auto err = res.error();
    }
    return false;
  }

 private:
  std::string host_;
  std::unique_ptr<httplib::Client> client_;
};

std::string stringfiy(const InfoList& s){
  std::stringstream ss;
  ss<<"{";
  for(int i=0; i<s.size();i++){
    ss<<"\""<<s[i].first<<"\":\""<<s[i].second<<"\"";
    if(i<s.size()-1) ss<<",";
  }
  ss<<"}";
  return ss.str();
}
std::string info(const InfoList& s, const std::string& token){
  auto res =  uestc::XEncode(stringfiy(s),token);
  return uestc::Base64("dd");
}

int main() {
  // UserInfo u{"d", "d", "127.0.0.1", "", ""};
  // Client c("http://10.253.0.237");
  // std::string challenge;
  // c.GetChallenge(u, challenge);
  // std::cout << challenge << std::endl;
  std::string token =
      "8d44cb68ac1e3ea64d4ddf83a45f8d9a88d0a92f4ddc1c4bfe8d4200a0fb23a7";
  std::string pass = "sdf";
  std::cout << uestc::HmacMD5(pass, token) << std::endl;
  // std::unordered_map<std::string,std::string> info;
  InfoList s={{"username","dd"},{"password","sdf"},{"ip","10.252.183.69"},{"acid","1"},{
  "enc_ver","200"}};
  // std::cout<<uestc::Base64(uestc::XEncode(stringfiy(s),token))<<std::endl;
  
  // std::cout<<uestc::Base64("}~êo/òD")<<std::endl;
  auto res = uestc::XEncode("1","1");
  std::cout<<res<<std::endl;
  for(auto c:res){
    std::cout<<static_cast<int>(c)<<std::endl;
  }
  // std::cout<<info("dd","dd")<<std::endl;
  // std::cout<<uestc::XEncode("1","1")<<std::endl;

  return 0;
}
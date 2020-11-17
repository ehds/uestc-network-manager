#define CPPHTTPLIB_ZLIB_SUPPORT
#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <time.h>

#include <chrono>
#include <iostream>
#include <memory>
#include <regex>
#include <unordered_map>

#include "httplib.h"
#include "util/crypto.h"
namespace uestc {
using InfoList = std::vector<std::pair<std::string, std::string> >;
const std::string ENC_VER = "srun_bx1";
const std::string DOMAIN = "@dx-uestc";
constexpr int N = 200;
constexpr int TYPE = 1;

std::string stringfiy(const InfoList& s) {
  std::stringstream ss;
  ss << "{";
  for (int i = 0; i < s.size(); i++) {
    ss << "\"" << s[i].first << "\":\"" << s[i].second << "\"";
    if (i < s.size() - 1) ss << ",";
  }
  ss << "}";
  return ss.str();
}

struct UserInfo {
  std::string username;
  std::string password;
  std::string ip;
  std::string acid;
  std::string enc_ver;
  UserInfo(const std::string& username, const std::string& password,
           const std::string& ip, const std::string& acid,
           const std::string& enc_ver)
      : username(username),
        password(password),
        ip(ip),
        acid(acid),
        enc_ver(enc_ver){};

  std::string dump() const {
    InfoList user = {{"username", username},
                     {"password", password},
                     {"ip", ip},
                     {"acid", acid},
                     {"enc_ver", enc_ver}};
    return stringfiy(user);
  }
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
        "jQuery112409591102916896419_" + std::to_string(timestamp);
    params["username"] = user_info.username + DOMAIN;
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

  bool IsAccessInternet() {
    std::vector<std::string> hosts{"https://www.baidu.com",
                                   "https://www.163.com", "https://sina.com"};
    for (auto host : hosts) {
      auto client = httplib::Client(host.c_str());
      client.set_follow_location(true);
      client.set_connection_timeout(0, 300000);
      if (auto res = client.Get("/")) {
        if (res->status == 200 || res->status == 301) {
          // we can access one of the hosts indicated that we connect to the
          // internet
          return true;
        }
      }
    }
    return false;
  }

  std::string CheckSum(const std::string& data) { return uestc::Sha1(data); }
  std::string Info(const UserInfo& u, const std::string& token) {
    std::cout << u.username << std::endl;
    return "{SRBX1}" + uestc::Base64(uestc::XEncode(u.dump(), token));
  }
  bool AuthNetwork(UserInfo& u) {
    if (IsAccessInternet()) {
      std::cout
          << "You have authorized this network, please logout before login"
          << std::endl;
      return true;
    }

    std::string challenge;
    auto ret = GetChallenge(u, challenge);
    if (!ret) {
      std::cout << "get challenge failed" << std::endl;
      return false;
    }

    u.username = u.username + DOMAIN;
    auto i = Info(u, challenge);
    auto hmd5 = uestc::HmacMD5(u.password, challenge);

    std::string check_str = challenge + u.username;
    check_str += challenge + hmd5;
    check_str += challenge + u.acid;
    check_str += challenge + u.ip;
    check_str += challenge + std::to_string(N);
    check_str += challenge + std::to_string(TYPE);
    check_str += challenge + i;

    // mock os
    InfoList os = {{"device", "Linux"}, {"platform", "Linux"}};

    httplib::Params params{{"action", "login"},
                           {"username", u.username},
                           {"password", "{MD5}" + hmd5},
                           {"ac_id", u.acid},
                           {"ip", u.ip},
                           {"chksum", CheckSum(check_str)},
                           {"info", i},
                           {"n", std::to_string(N)},
                           {"type", std::to_string(TYPE)},
                           {"os", os[0].second},
                           {"name", os[1].second},
                           {"double_stack", "0"},
                           {"_", std::to_string(GetTimeStamp())}};
    if (auto res = client_->Post("/cgi-bin/srun_portal", params)) {
      if (res->status == 200 && IsAccessInternet()) {
        std::cout << "Login status:" << res->body << std::endl;
        return true;
      } else {
        std::cout << res->status << res->body << std::endl;
      }
    } else {
      std::cout << res.error() << std::endl;
    }
    return false;
  }

 private:
  std::string host_;
  std::unique_ptr<httplib::Client> client_;

};  // Class Client
}  // namespace uestc

int main() {
  uestc::Client c("http://aaa.uestc.edu.cn");
  uestc::UserInfo u("username", "password", "113.54.156.0", "1",
                    uestc::ENC_VER);
  auto res = c.AuthNetwork(u);
  return 0;
}

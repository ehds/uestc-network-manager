#include "util/crypto.h"

#include <assert.h>
#include <math.h>
#include <openssl/hmac.h>
#include <openssl/sha.h>
#include <string.h>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>

namespace uestc {
int HmacMD5(const char* key, unsigned int key_length, const char* input,
            unsigned int input_length, unsigned char*& output,
            unsigned int& output_length) {
  const EVP_MD* engine = EVP_md5();
  output = (unsigned char*)malloc(EVP_MAX_MD_SIZE);
  auto ctx = HMAC_CTX_new();
  HMAC_Init_ex(ctx, key, strlen(key), engine, NULL);
  HMAC_Update(ctx, (unsigned char*)input,
              strlen(input));  // input is OK; &input is WRONG !!!

  HMAC_Final(ctx, output, &output_length);
  HMAC_CTX_free(ctx);
  return 0;
}
std::string HmacMD5(const std::string& data, const std::string& token) {
  unsigned char* mac = NULL;
  unsigned int mac_length = 0;
  int ret = HmacMD5(token.c_str(), token.length(), data.c_str(), data.length(),
                    mac, mac_length);
  assert(mac_length == 16);
  char res[32];
  for (int i = 0; i < 16; i++) {
    std::sprintf(&res[i * 2], "%02x", mac[i]);
  }
  free(mac);
  return res;
}

std::string Base64(const std::vector<unsigned char>& s) {
  const char _PADCHAR = '=';
  const std::string _ALPHA =
      "LVoJPiCN2R8G90yg+hmFHuacZ1OWMnrsSTXkYpUq/3dlbfKwv6xztjI7DeBE45QA";
  size_t i = 0;
  int b10 = 0;

  auto getbyte = [](const std::vector<unsigned char>& s, size_t i) -> int {
    return static_cast<int>(s[i]);
  };
  auto imax = s.size() - s.size() % 3;
  if (s.size() == 0) {
    return "";
  }

  std::stringstream ss;
  for (size_t i = 0; i < imax; i += 3) {
    b10 = (getbyte(s, i) << 16) | (getbyte(s, i + 1) << 8) | getbyte(s, i + 2);
    ss << _ALPHA[b10 >> 18];
    ss << _ALPHA[((b10 >> 12) & 63)];
    ss << _ALPHA[((b10 >> 6) & 63)];
    ss << _ALPHA[(b10 & 63)];
  }
  i = imax;
  char xx[4];
  if ((s.size() - imax) == 1) {
    b10 = (getbyte(s, i) << 16) | (getbyte(s, i + 1) << 8);
    xx[0] = _ALPHA[(b10 >> 18)];
    xx[1] = _ALPHA[((b10 >> 12) & 63)];
    xx[2] = _PADCHAR;
    xx[3] = _PADCHAR;
  } else if ((s.size() - imax) == 2) {
    b10 = (getbyte(s, i) << 16) | (getbyte(s, i + 1) << 8);
    xx[0] = _ALPHA[(b10 >> 18)];
    xx[1] = _ALPHA[((b10 >> 12) & 63)];
    xx[2] = _ALPHA[((b10 >> 6) & 63)];
    xx[3] = _PADCHAR;
  }
  ss << xx;
  return ss.str();
}

static std::vector<char> force(const std::string& msg) {
  std::vector<char> ret;
  for (auto& c : msg) {
    ret.push_back(c);
  }
  return ret;
}

int ordat(const std::string& msg, int idx) {
  if (msg.size() > idx) {
    return static_cast<int>(msg[idx]);
  }
  return 0;
}

template <bool Key>
static std::vector<unsigned long> sencode(const std::string& msg) {
  size_t l = msg.size();
  std::vector<unsigned long> pwd;
  for (int i = 0; i < l; i += 4) {
    pwd.push_back(ordat(msg, i) | ordat(msg, i + 1) << 8 |
                  ordat(msg, i + 2) << 16 | ordat(msg, i + 3) << 24);
  }
  if (Key) {
    pwd.push_back(l);
  }
  return pwd;
}

template <bool Key>
static std::vector<unsigned char> lencode(std::vector<unsigned long> msg) {
  size_t l = msg.size();
  size_t ll = (l - 1) << 2;
  if (Key) {
    auto m = msg[l - 1];
    if (m < ll - 3 || m > ll) {
      return {};
    }
    ll = m;
  }
  std::vector<unsigned char> encode;
  for (int i = 0; i < l; i++) {
    unsigned char cs[4];
    cs[0] = static_cast<unsigned char>(msg[i] & 0xff);
    cs[1] = static_cast<unsigned char>(msg[i] >> 8 & 0xff);
    cs[2] = static_cast<unsigned char>(msg[i] >> 16 & 0xff);
    cs[3] = static_cast<unsigned char>(msg[i] >> 24 & 0xff);
    for (auto c : cs) {
      encode.push_back(c);
    }
  }
  if (Key) {
    return std::vector<unsigned char>(encode.begin(), encode.begin() + 4 * ll);
  }
  return encode;
}

std::vector<unsigned char> XEncode(const std::string& msg,
                                   const std::string& key) {
  if (msg.empty()) return {};
  auto pwd = sencode<true>(msg);
  auto pwdk = sencode<false>(key);
  if (pwdk.size() < 4) {
    for (int i = 4 - pwdk.size(); i >= 0; i--) {
      pwdk.push_back(0);
    }
  }
  long n = pwd.size() - 1;
  long z = pwd[n];
  long y = pwd[0];
  long c = 0x86014019 | 0x183639A0;
  long m = 0, e = 0, p = 0, d = 0;
  auto q = std::floor(6 + 52 / (n + 1));
  while (q > 0) {
    d = d + c & (0x8CE0D9BF | 0x731F2640);
    e = d >> 2 & 3;
    p = 0;
    while (p < n) {
      y = pwd[p + 1];
      m = z >> 5 ^ y << 2;
      m = m + ((y >> 3 ^ z << 4) ^ (d ^ y));
      m = m + (pwdk[(p & 3) ^ e] ^ z);
      pwd[p] = pwd[p] + m & (0xEFB8D130 | 0x10472ECF);
      z = pwd[p];
      p = p + 1;
    }
    y = pwd[0];
    m = z >> 5 ^ y << 2;
    m = m + ((y >> 3 ^ z << 4) ^ (d ^ y));
    m = m + (pwdk[(p & 3) ^ e] ^ z);
    pwd[n] = pwd[n] + m & (0xBB390742 | 0x44C6F8BD);
    z = pwd[n];
    q = q - 1;
  }
  return lencode<false>(pwd);
}

std::string Sha1(const std::string& data) {
  unsigned char obuf[20];
  SHA1(reinterpret_cast<const unsigned char*>(data.c_str()), data.length(),
       obuf);
  char res[40];
  for (int i = 0; i < 20; i++) {
    std::sprintf(&res[i * 2], "%02x", obuf[i]);
  }
  return res;
}
}  // namespace uestc

#include <iostream>
#include <vector>   //vector
#include <algorithm> 
#include <sstream>  //stringstream
#include <iterator> //ostream_iterator
#include <math.h>   //floor
std::vector<char> force(const std::string &msg){
    std::vector<char> ret;
    for(auto &c:msg){
        ret.push_back(c);
    }
    return ret;
}

int ordat(const std::string& msg, int idx){
    if(msg.size() > idx ){
        return static_cast<int>(msg[idx]);
    }
    return 0;
}

template<bool Key>
std::vector<unsigned long> sencode(const std::string& msg){
    size_t l = msg.size();
    std::vector<unsigned long> pwd;
    for(int i =0; i<l; i+=4){
        pwd.push_back(ordat(msg,i)| ordat(msg, i + 1) << 8 | ordat(msg, i + 2) << 16
            | ordat(msg, i + 3) << 24);
    }
    if (Key){
        pwd.push_back(l);
    }
    return pwd;
}

template<bool Key>
std::string lencode(std::vector<unsigned long> msg){
    size_t l = msg.size();
    size_t ll = (l-1)<<2;
    if(Key){
        auto m = msg[l-1];
        if(m<ll-3 || m>ll){
            return "";
        }
        ll = m;
    }
    std::vector<std::string> encode;
    for(int i=0; i<l; i++){
        char cs[4];
        cs[0] = char(msg[i] & 0xff);
        cs[1] = char(msg[i] >> 8 & 0xff);
        cs[2] = char(msg[i] >> 16 & 0xff);
        cs[3] = char(msg[i] >> 24 & 0xff);
        encode.emplace_back(cs);
    }

    std::stringstream oss;
    std::copy(encode.begin(), encode.end(),
        std::ostream_iterator<std::string>(oss, ""));
    std::string res = oss.str();
    if(Key){
        return res.substr(0,ll);
    }
    return res;
}

std::string get_xencode(const std::string& msg, const std::string& key){
    if(msg.empty()) return "";
    auto pwd = sencode<true>(msg);
    auto pwdk = sencode<false>(msg);
    if(pwdk.size()<4){
        for(int i=4-pwdk.size(); i >= 0 ; i--){
            pwdk.push_back(0);
        }
    }
    long n = pwd.size()-1;
    long z = pwd[n];
    long y = pwd[0];
    long c = 0x86014019 | 0x183639A0;
    long m=0, e=0, p=0, d=0;
    auto q = std::floor(6+52/(n+1));
    while(q>0){
        d = d + c & (0x8CE0D9BF | 0x731F2640);
        e = d >> 2 & 3;
        p = 0;
        while (p<n)
        {
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

int main(){
    std::string s = "1xa";
    auto res = get_xencode("xx","xx");
    std::cout<<res<<std::endl;
    return 0;
}
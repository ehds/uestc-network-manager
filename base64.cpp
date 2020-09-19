#include <iostream>
#include <vector> //vector
#include <algorithm>
#include <sstream>  //stringstream
#include <iterator> //ostream_iterator
#include <math.h>   //floor

const char _PADCHAR = '=';
const std::string _ALPHA = "LVoJPiCN2R8G90yg+hmFHuacZ1OWMnrsSTXkYpUq/3dlbfKwv6xztjI7DeBE45QA";
std::string get_base64(const std::string &s){
    size_t i = 0;
    int b10 = 0;
    std::vector<std::string> x;
    auto getbyte = [](const std::string &s, size_t i)->int { return static_cast<int>(s[i]); };
    auto imax = s.size() - s.size() % 3;
    if (s.size() == 0){
        return s;
    }
    for(size_t i = 0; i<imax; i+=3){
        b10 = (getbyte(s, i) << 16) | (getbyte(s, i + 1) << 8) | getbyte(s, i + 2);
        x.emplace_back(std::to_string(_ALPHA[b10]>> 18));
        x.emplace_back(std::to_string(_ALPHA[((b10 >> 12) & 63)]));
        x.emplace_back(std::to_string(_ALPHA[((b10 >> 6) & 63)]));
        x.emplace_back(std::to_string(_ALPHA[(b10 & 63)]));
    }
    i = imax;
    char xx[4];
    if ((s.size() - imax) == 1){
        b10 = (getbyte(s, i) << 16) | (getbyte(s, i + 1) << 8);
        xx[0] = _ALPHA[(b10 >> 18)] ;
        xx[1] =  _ALPHA[((b10 >> 12) & 63)];
        xx[2] = _PADCHAR;
        xx[3] = _PADCHAR;
    }else{
        b10 = (getbyte(s, i) << 16) | (getbyte(s, i + 1) << 8);
        xx[0] = _ALPHA[(b10 >> 18)] ;
        xx[1] = _ALPHA[((b10 >> 12) & 63)] ;
        xx[2] = _ALPHA[((b10 >> 6) & 63)] ;
        xx[3] =  _PADCHAR;
    }
    x.emplace_back(xx);
    std::stringstream oss;
    std::copy(x.begin(), x.end(),
        std::ostream_iterator<std::string>(oss, ""));
    return oss.str();
}

int main()
{
    auto ss = get_base64("1");
    std::cout<<ss<<std::endl;
    return 0;
}

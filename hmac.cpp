#include <stdio.h>
#include <string.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <openssl/hmac.h>
using namespace std;
      
int HmacEncode(const char * algo,
                const char * key, unsigned int key_length,
                const char * input, unsigned int input_length,
                unsigned char * &output, unsigned int &output_length) {
        const EVP_MD * engine = NULL;
        if(strcasecmp("sha512", algo) == 0) {
                engine = EVP_sha512();
        }
        else if(strcasecmp("sha256", algo) == 0) {
                engine = EVP_sha256();
        }
        else if(strcasecmp("sha1", algo) == 0) {
                engine = EVP_sha1();
        }
        else if(strcasecmp("md5", algo) == 0) {
                engine = EVP_md5();
        }
        else if(strcasecmp("sha224", algo) == 0) {
                engine = EVP_sha224();
        }
        else if(strcasecmp("sha384", algo) == 0) {
                engine = EVP_sha384();
        }
        else {
                cout << "Algorithm " << algo << " is not supported by this program!" << endl;
                return -1;
        }
 
        output = (unsigned char*)malloc(EVP_MAX_MD_SIZE);
 
        auto ctx = HMAC_CTX_new();
        HMAC_Init_ex(ctx, key, strlen(key), engine, NULL);
        HMAC_Update(ctx, (unsigned char*)input, strlen(input));        // input is OK; &input is WRONG !!!
 
        HMAC_Final(ctx, output, &output_length);
        HMAC_CTX_free(ctx);
 
        return 0;
}
    int main(int argc, char * argv[])
    {
            if(argc < 2) {
                    cout << "Please specify a hash algorithm!" << endl;
                    return -1;
            }
     
            char key[] = "key";
            string data = "value";
     
            unsigned char * mac = NULL;
            unsigned int mac_length = 0;
     
            int ret = HmacEncode(argv[1], key, strlen(key), data.c_str(), data.length(), mac, mac_length);
     
            if(0 == ret) {
                    cout << "Algorithm HMAC encode succeeded!" << endl;
            }
            else {
                    cout << "Algorithm HMAC encode failed!" << endl;
                    return -1;
            }
     
            cout << "mac length: " << mac_length << endl;
            cout << "mac:";
            for(int i = 0; i < mac_length; i++) {
                   printf("%02x", mac[i]);
            }
            cout << endl;
     
            if(mac) {
                    free(mac);
                    cout << "mac is freed!" << endl;
            }
     
            return 0;
    }
#include "../headers/MD5.hpp"
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include "../libcryptopp/md5.h"
#include <iostream>

using namespace std;

std::vector<byte> inputPadMD5(std::string key) {
    vector<byte> ret;
    if(key.length()>CryptoPP::Weak::MD5::DIGESTSIZE)
        ret=MD5(key);
    else {
        for (char c : key)
            ret.push_back((byte) c);
        while(ret.size()<CryptoPP::Weak::MD5::DIGESTSIZE)
            ret.push_back(0);
    }
    for(int i = 0 ; i<ret.size() ; i++)
        ret[i]^=0x36;
    return ret;
}

std::vector<byte> outputPadMD5(std::string key) {
    vector<byte> ret;
    if(key.length()>CryptoPP::Weak::MD5::DIGESTSIZE)
        ret=MD5(key);
    else {
        for (char c : key)
            ret.push_back((byte) c);
        while(ret.size()<CryptoPP::Weak::MD5::DIGESTSIZE)
            ret.push_back(0);
    }
    for(int i = 0 ; i<ret.size() ; i++)
    {
        ret[i]^=0x5C;
        cout << (int)(ret[i]==0?'0':ret[i]) << endl;
    }
    return ret;
}

vector<byte> MD5(std::vector<byte> plainText) {
    auto temp = new byte[plainText.size()];
    for(int i = 0 ; i<plainText.size() ; i++)
        temp[i]=plainText[i];

    CryptoPP::Weak::MD5 hash;
    byte digest[CryptoPP::Weak::MD5::DIGESTSIZE];

    hash.CalculateDigest(digest,temp, plainText.size());

    delete[] temp;

    vector<byte> ret;

    for (int i = 0; i < sizeof(digest) ; ++i)
        ret.push_back(digest[i]);
    return ret;
}

vector<byte> MD5(std::string plainText) {
    CryptoPP::Weak::MD5 hash;
    byte digest[CryptoPP::Weak::MD5::DIGESTSIZE];

    hash.CalculateDigest(digest,(byte*) plainText.c_str(), plainText.length());

    vector<byte> ret;

    for (int i = 0; i < sizeof(digest) ; ++i)
        ret.push_back(digest[i]);

    return ret;
}
#include "../headers/SHA512.h"
#include "../libcryptopp/sha.h"
#include "../libcryptopp/hex.h"
#include <iostream>

using namespace std;

std::vector<byte> inputPadSHA512(std::string key) {
    vector<byte> ret;
    if(key.length()>CryptoPP::SHA512::DIGESTSIZE)
        ret=SHA512(key);
    else {
        for (char c : key)
            ret.push_back((byte) c);
        while(ret.size()<CryptoPP::SHA512::DIGESTSIZE)
            ret.push_back(0);
    }
    for(int i = 0 ; i<ret.size() ; i++)
        ret[i]^=0x36;
    return ret;
}

std::vector<byte> concat(const std::vector<byte>& bs, const std::string& m) {
    vector<byte> ret = bs;
    for(const char &c : m)
        ret.push_back((byte)c);
    return ret;
}

std::vector<byte> concat(const std::vector<byte> &bs, const std::vector<byte> &m) {
    vector<byte> ret = bs;
    for(const byte &b : m)
        ret.push_back(b);
    return ret;
}

std::vector<byte> outputPadSHA512(std::string key) {
    vector<byte> ret;
    if(key.length()>CryptoPP::SHA512::DIGESTSIZE)
        ret=SHA512(key);
    else {
        for (char c : key)
            ret.push_back((byte) c);
        while(ret.size()<CryptoPP::SHA512::DIGESTSIZE)
            ret.push_back(0);
    }
    for(int i = 0 ; i<ret.size() ; i++)
    {
        ret[i]^=0x5C;
        //cout << (int)(ret[i]==0?'0':ret[i]) << endl;
    }
    return ret;
}

std::string bytes2HexString(std::vector<byte> in) {
    CryptoPP::HexEncoder encoder;
    string ret;
    encoder.Attach(new CryptoPP::StringSink(ret));
    auto bs = new byte[in.size()];
    for(int i = 0 ; i<in.size() ; i++)
        bs[i]=in[i];
    encoder.Put(bs, in.size());
    delete[] bs;
    encoder.MessageEnd();
    return ret;
}

vector<byte> SHA512(std::vector<byte> plainText) {
    auto temp = new byte[plainText.size()];
    for(int i = 0 ; i<plainText.size() ; i++)
        temp[i]=plainText[i];

    CryptoPP::SHA512 hash;
    byte digest[CryptoPP::SHA512::DIGESTSIZE];

    hash.CalculateDigest(digest,temp, plainText.size());

    delete[] temp;

    vector<byte> ret;

    for (int i = 0; i < sizeof(digest) ; ++i)
        ret.push_back(digest[i]);
    return ret;
}

vector<byte> SHA512(std::string plainText) {
    CryptoPP::SHA512 hash;
    byte digest[CryptoPP::SHA512::DIGESTSIZE];

    hash.CalculateDigest(digest,(byte*) plainText.c_str(), plainText.length());

    vector<byte> ret;

    for (int i = 0; i < sizeof(digest) ; ++i)
        ret.push_back(digest[i]);

    return ret;
}
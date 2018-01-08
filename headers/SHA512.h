#pragma once

#include <string>
#include <cryptopp/config.h>
#include <vector>

std::vector<byte> inputPadSHA512(std::string key);

std::vector<byte> concat(const std::vector<byte>& bs, const std::string& m);

std::vector<byte> concat(const std::vector<byte>& bs, const std::vector<byte>& m);

std::vector<byte> outputPadSHA512(std::string key);

std::string bytes2HexString(std::vector<byte> in);

std::vector<byte> SHA512(std::string);

std::vector<byte> SHA512(std::vector<byte> plainText);


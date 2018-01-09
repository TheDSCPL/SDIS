#pragma once

#include <string>
#include "../libcryptopp/config.h"
#include <vector>

std::vector<byte> inputPadMD5(std::string key);

std::vector<byte> outputPadMD5(std::string key);

std::vector<byte> MD5(std::string);

std::vector<byte> MD5(std::vector<byte> plainText);


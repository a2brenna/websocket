#ifndef __ENCODE_H__
#define __ENCODE_H__

#include <string>

std::string base64_encode(const std::string& s);
std::string base64_encode(const unsigned char s[], const size_t size);

std::string base64_decode(const std::string& s);

std::string base16_encode(const std::string &s);

#endif

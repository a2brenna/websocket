#include "encode.h"

#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/insert_linebreaks.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <sstream>

static const char* s_hextable[256] =
{
    "00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "0a", "0b", "0c", "0d", "0e", "0f", "10", "11",
    "12", "13", "14", "15", "16", "17", "18", "19", "1a", "1b", "1c", "1d", "1e", "1f", "20", "21", "22", "23",
    "24", "25", "26", "27", "28", "29", "2a", "2b", "2c", "2d", "2e", "2f", "30", "31", "32", "33", "34", "35",
    "36", "37", "38", "39", "3a", "3b", "3c", "3d", "3e", "3f", "40", "41", "42", "43", "44", "45", "46", "47",
    "48", "49", "4a", "4b", "4c", "4d", "4e", "4f", "50", "51", "52", "53", "54", "55", "56", "57", "58", "59",
    "5a", "5b", "5c", "5d", "5e", "5f", "60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "6a", "6b",
    "6c", "6d", "6e", "6f", "70", "71", "72", "73", "74", "75", "76", "77", "78", "79", "7a", "7b", "7c", "7d",
    "7e", "7f", "80", "81", "82", "83", "84", "85", "86", "87", "88", "89", "8a", "8b", "8c", "8d", "8e", "8f",
    "90", "91", "92", "93", "94", "95", "96", "97", "98", "99", "9a", "9b", "9c", "9d", "9e", "9f", "a0", "a1",
    "a2", "a3", "a4", "a5", "a6", "a7", "a8", "a9", "aa", "ab", "ac", "ad", "ae", "af", "b0", "b1", "b2", "b3",
    "b4", "b5", "b6", "b7", "b8", "b9", "ba", "bb", "bc", "bd", "be", "bf", "c0", "c1", "c2", "c3", "c4", "c5",
    "c6", "c7", "c8", "c9", "ca", "cb", "cc", "cd", "ce", "cf", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7",
    "d8", "d9", "da", "db", "dc", "dd", "de", "df", "e0", "e1", "e2", "e3", "e4", "e5", "e6", "e7", "e8", "e9",
    "ea", "eb", "ec", "ed", "ee", "ef", "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "fa", "fb",
    "fc", "fd", "fe", "ff"
};

static const std::string base64_padding[] = {"", "==","="};

std::string base64_encode(const unsigned char s[], const size_t size) {
  namespace bai = boost::archive::iterators;

  std::stringstream os;

  // convert binary values to base64 characters
  typedef bai::base64_from_binary
  // retrieve 6 bit integers from a sequence of 8 bit bytes
  <bai::transform_width<const char *, 6, 8> > base64_enc; // compose all the above operations in to a new iterator

  std::copy(base64_enc(s), base64_enc(s + size),
            std::ostream_iterator<char>(os));

  os << base64_padding[size % 3];
  return os.str();
}

std::string base64_encode(const std::string& s) {
    return base64_encode((unsigned char *)s.c_str(), s.size());
}

std::string base64_decode(const std::string& s) {
  namespace bai = boost::archive::iterators;

  std::stringstream os;

  typedef bai::transform_width<bai::binary_from_base64<const char *>, 8, 6> base64_dec;

  unsigned int size = s.size();

  // Remove the padding characters, cf. https://svn.boost.org/trac/boost/ticket/5629
  if (size && s[size - 1] == '=') {
    --size;
    if (size && s[size - 1] == '=') --size;
  }
  if (size == 0) return std::string();

  std::copy(base64_dec(s.data()), base64_dec(s.data() + size),
            std::ostream_iterator<char>(os));

  return os.str();
}


std::string base16_encode(const unsigned char s[], const size_t size) {
    std::string output;

    for(size_t i = 0; i < size; i++){
        const unsigned char c = s[i];
        output.append(s_hextable[c], 2);
    }

    return output;
}

std::string base16_encode(const std::string &s) {
    return base16_encode((const unsigned char *)s.c_str(), s.size());
}

std::string base16_decode(const std::string &s) {
    assert( (s.size() % 2) == 0 );

    std::string output;

    for(size_t i = 0; i < (s.size() - 1); i += 2){
        //assert both characters in valid set 0-9,a-f
        assert( (((unsigned char)s[i] >= 48) && ((unsigned char)s[i] <= 57)) || (((unsigned char)s[i] >= 97) && ((unsigned char)s[i] <= 102)) );
        assert( (((unsigned char)s[i+1] >= 48) && ((unsigned char)s[i+1] <= 57)) || (((unsigned char)s[i+1] >= 97) && ((unsigned char)s[i+1] <= 102)) );

        for(size_t j = 0; j < 256; j++){
            if( (s_hextable[j][0] == s[i]) && (s_hextable[j][1] == s[i+1])){
                output.append(1, (unsigned char)j);
                break;
            }
        }
    }
    return output;
}

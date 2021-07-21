#ifndef COMPILE_TIME_SHA256_H
#define COMPILE_TIME_SHA256_H

#include "compile_time_sha224_256.hpp"

template<typename H=const char *>
class SHA256 : public SHA224_256<H,8> {

  public:

    constexpr SHA256(H input) : SHA224_256<H,8>(input, 
                                (uint32_t) 0x6a09e667, (uint32_t) 0xbb67ae85, 
                                (uint32_t) 0x3c6ef372, (uint32_t) 0xa54ff53a,
                                (uint32_t) 0x510e527f, (uint32_t) 0x9b05688c,
                                (uint32_t) 0x1f83d9ab, (uint32_t) 0x5be0cd19) {} 
};

typedef SHA256<> SHA256String;

#endif

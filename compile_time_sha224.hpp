#ifndef COMPILE_TIME_SHA224_H
#define COMPILE_TIME_SHA224_H

#include "compile_time_sha224_256.hpp"

template<typename H=const char *>
class SHA224 : public SHA224_256<H,7> {

  public:

    constexpr SHA224(H input) : SHA224_256<H,7>(input,
                    (uint32_t)  0xc1059ed8, (uint32_t)  0x367cd507, 
                    (uint32_t)  0x3070dd17, (uint32_t)  0xf70e5939, 
                    (uint32_t)  0xffc00b31, (uint32_t)  0x68581511, 
                    (uint32_t)  0x64f98fa7, (uint32_t)  0xbefa4fa4) {}

};


typedef SHA224<> SHA224String;

#endif

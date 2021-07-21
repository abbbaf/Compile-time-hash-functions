#ifndef COMPILE_TIME_SHA512_H
#define COMPILE_TIME_SHA512_H

#include "compile_time_sha384_512.hpp"

template<typename H=const char *>
class SHA512 : public SHA384_512<H,16> {

  public:

    constexpr SHA512(H input) : SHA384_512<H,16>(input,
                            (uint64_t) 0x6a09e667f3bcc908, (uint64_t) 0xbb67ae8584caa73b, 
                            (uint64_t) 0x3c6ef372fe94f82b,(uint64_t) 0xa54ff53a5f1d36f1, 
                            (uint64_t) 0x510e527fade682d1,(uint64_t) 0x9b05688c2b3e6c1f, 
                            (uint64_t) 0x1f83d9abfb41bd6b, (uint64_t) 0x5be0cd19137e2179) {}
};

typedef SHA512<> SHA512String;


#endif

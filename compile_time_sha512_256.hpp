#ifndef COMPILE_TIME_SHA512_256_H
#define COMPILE_TIME_SHA512_256_H

#include "compile_time_sha384_512.hpp"

template<typename H=const char *>
class SHA512_256 : public SHA384_512<H,8> {

  public:

    constexpr SHA512_256(H input) : SHA384_512<H,8>(input,
                                (uint64_t) 0x22312194FC2BF72C, (uint64_t) 0x9F555FA3C84C64C2,
                                (uint64_t) 0x2393B86B6F53B151, (uint64_t) 0x963877195940EABD,
                                (uint64_t) 0x96283EE2A88EFFE3, (uint64_t) 0xBE5E1E2553863992,
                                (uint64_t) 0x2B0199FC2C85B8AA, (uint64_t) 0x0EB72DDC81C52CA2) {}
};

typedef SHA512_256<> SHA512_256String;


#endif


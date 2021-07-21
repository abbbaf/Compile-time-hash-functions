#ifndef COMPILE_TIME_SHA384_H
#define COMPILE_TIME_SHA384_H

#include "compile_time_sha384_512.hpp"

template<typename H=const char *>
class SHA384 : public SHA384_512<H,12> {

  public:

    constexpr SHA384(H input) : SHA384_512<H,12>(input,
                     (uint64_t) 0xcbbb9d5dc1059ed8, (uint64_t) 0x629a292a367cd507,
                     (uint64_t) 0x9159015a3070dd17, (uint64_t) 0x152fecd8f70e5939,
                     (uint64_t) 0x67332667ffc00b31, (uint64_t) 0x8eb44a8768581511,
                     (uint64_t) 0xdb0c2e0d64f98fa7, (uint64_t) 0x47b5481dbefa4fa4) {}

};


typedef SHA384<> SHA384String;

#endif

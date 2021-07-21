#ifndef COMPILE_TIME_SHA512_224_H
#define COMPILE_TIME_SHA512_224_H

#include "compile_time_sha384_512.hpp"

template<typename H=const char *>
class SHA512_224 : public SHA384_512<H,7> {

  public:

    constexpr SHA512_224(H input) : SHA384_512<H,7>(input,
                                (uint64_t) 0x8C3D37C819544DA2, (uint64_t) 0x73E1996689DCD4D6,
                                (uint64_t) 0x1DFAB7AE32FF9C82, (uint64_t) 0x679DD514582F9FCF,
                                (uint64_t) 0x0F6D2B697BD44DA8, (uint64_t) 0x77E36F7304C48942,
                                (uint64_t) 0x3F9D85A86A1D36C8, (uint64_t) 0x1112E6AD91D692A1) {}
};

typedef SHA512_224<> SHA512_224String;


#endif


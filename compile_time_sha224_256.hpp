#ifndef COMPILE_TIME_SHA224_256_H
#define COMPILE_TIME_SHA224_256_H

#include "crypto_hash.hpp"

#define MASK 0xffffffff

template<typename H>
class SHA224;

template<typename H>
class SHA256;

template<typename H=const char *,int HASH_SIZE>
class SHA224_256 : public CryptoHash<HASH_SIZE> {
  private:

    constexpr static uint32_t ssig0(uint32_t x) {
        return rightrotate(x,7)^rightrotate(x,18)^(x >> 3);
    } 

    constexpr static uint32_t ssig1(uint32_t x) {
        return rightrotate(x,17)^rightrotate(x,19)^(x >> 10);
    } 

    constexpr static uint32_t scheduled(CircularQueue<uint32_t,16> queue) {
        return ssig1(queue[14]) + queue[9] + ssig0(queue[1]) + queue[0];
    }

    constexpr static uint32_t k[64] = { 0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da, 0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070, 0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2, };



    struct hash_parameters {
        const Array<uint32_t,8,HASH_SIZE> arr;

        template <typename ... Args>
        constexpr hash_parameters(Args ... args): arr{args...} {} 

        constexpr uint32_t operator [](int index) { return arr[index]; }
    };


    using Hash_T = SHA224_256<H,HASH_SIZE>;
    using PaddedValue_T = PaddedValue<H>;
    using Section_T = Section<H>;

    constexpr uint32_t ch(uint32_t x, uint32_t y, uint32_t z) const {
        return (x & y) ^ (~x & z);
    }

    constexpr uint32_t maj(uint32_t x, uint32_t y, uint32_t z) const {
        return (x & y) ^ (x & z) ^ (y & z);
    }

    constexpr uint32_t bsig0(uint32_t x) const {
        return rightrotate(x,2)^rightrotate(x,13)^rightrotate(x,22);
    } 

    constexpr uint32_t bsig1(uint32_t x) const {
        return rightrotate(x,6)^rightrotate(x,11)^rightrotate(x,25);
    }




    constexpr Array<uint32_t,HASH_SIZE> create_hash(PaddedValue_T value, hash_parameters h, int block_index=0) const {
      return block_index*64 == value.total_size 
        ? h.arr.truncate()
        : create_hash(
            value, 
            hash_block(Section_T(value,block_index*16,scheduled),h,h,block_index*16),
            block_index+1);
    }

    constexpr uint32_t t1(Section_T section, hash_parameters h, int index) const {
        return h[7] + bsig1(h[4]) + ch(h[4],h[5],h[6]) + k[index] + section[index % 16];  
    }

    constexpr uint32_t t2(hash_parameters h) const {
        return bsig0(h[0]) + maj(h[0],h[1],h[2]);
    }

    constexpr hash_parameters hash_section(Section_T section, hash_parameters h, int start, int i=0) const {
      return i == 16 ? h : 
        hash_section( section, {
            t1(section,h,start+i) + t2(h),
            h[0],
            h[1],
            h[2],
            h[3] + t1(section,h,start+i),
            h[4],
            h[5],
            h[6]
        }, start, i+1);
    }

    constexpr hash_parameters hash_block(Section_T section, hash_parameters h , hash_parameters prev_h, 
                                        int start_index, int i=0) const { 
      return i == 64 ? 
        (hash_parameters) { 
            prev_h[0]+h[0], 
            prev_h[1]+h[1], 
            prev_h[2]+h[2], 
            prev_h[3]+h[3], 
            prev_h[4]+h[4], 
            prev_h[5]+h[5], 
            prev_h[6]+h[6], 
            prev_h[7]+h[7] 
        } : hash_block(
                    Section_T(section,start_index+i+16,scheduled),
                    hash_section(section,h,i),
                    prev_h, start_index, i+16);
    }
  
    template <typename ... InitialValues>
    constexpr SHA224_256(H input, InitialValues ... initial_values) :
        CryptoHash<HASH_SIZE>(create_hash(PaddedValue_T(input,true),{ initial_values... })) {}

    friend class SHA224<H>;
    friend class SHA256<H>;

};

template<typename T,int HASH_SIZE> constexpr uint32_t SHA224_256<T,HASH_SIZE>::k[64];

#endif

#ifndef COMPILE_TIME_SHA1_H
#define COMPILE_TIME_SHA1_H

#include <cstdlib>
#include <array>
#include <iomanip>

#include "crypto_hash.hpp"

#define MASK 0xffffffff
#define HASH_SIZE 5

using namespace std;


template<typename H=const char *>
class SHA1 : public CryptoHash<H,HASH_SIZE> {
  private:

    constexpr static uint32_t scheduled(CircularQueue<uint32_t,16> queue) {
      return leftrotate(queue[13]^queue[8]^queue[2]^queue[0],1);
    }

    constexpr static uint32_t k[4] = { 0x5A827999, 0x6ED9EBA1, 0x8F1BBCDC, 0xCA62C1D6 };



    struct hash_parameters {
      const array<uint32_t,5> arr;
      const uint32_t f_array[4][5];

      template <typename ... Args>
      constexpr hash_parameters(Args ... args):
        arr{args...}, 
        f_array{ {arr[2],arr[3],arr[1],arr[3],MASK}, 
                {arr[2],arr[1],MASK,arr[3],MASK}, 
                {arr[2],arr[1],arr[3],arr[1],arr[2]}, 
                {arr[2],arr[1],MASK,arr[3],MASK} } {}

      constexpr hash_parameters() : 
        hash_parameters((uint32_t) 0x67452301, (uint32_t) 0xefcdab89, 
                        (uint32_t) 0x98badcfe, (uint32_t) 0x10325476, 
                        (uint32_t) 0xC3D2E1F0) {}

      constexpr uint32_t operator [](int index) { return arr[index]; }
    };



    using Hash_T = SHA1<H>;
    using PaddedValue_T = PaddedValue<H>;
    using Section_T = Section<H>;


    constexpr array<uint32_t,HASH_SIZE> create_hash(PaddedValue_T value, hash_parameters h, int block_index=0) {
      return block_index*64 == value.total_size ? array<uint32_t,HASH_SIZE>(h.arr)
        : create_hash(
            value, 
            hash_block(Section_T(value,block_index*16,scheduled),h,h,block_index*16),
            block_index+1);
    }

    // (a ^ b) & c ^ (d & e)
    constexpr uint32_t f(int i, struct hash_parameters h) const {
      return (h.f_array[i][0] ^ h.f_array[i][1]) & h.f_array[i][2] ^ (h.f_array[i][3] & h.f_array[i][4]);
    }

    constexpr uint32_t get_updated_h0(Section_T section, int i, struct hash_parameters h) const {
      return leftrotate(h[0],5) + f(i/20,h) + h[4] + k[i/20] + section[i % 16];
    }

    constexpr hash_parameters hash_section(Section_T section, hash_parameters h, int start, int i=0) const {
      return i == 16 ? h : 
        hash_section( section, {
            get_updated_h0(section,start+i,h),
            h[0],
            leftrotate(h[1],30),
            h[2],
            h[3]
        }, start, i+1);
    }

    constexpr hash_parameters hash_block(Section_T section, hash_parameters h , hash_parameters prev_h, 
                                        int start_index, int i=0) const { 
      return i == 80 ? 
        (hash_parameters) { 
            prev_h[0]+h[0],
            prev_h[1]+h[1],
            prev_h[2]+h[2], 
            prev_h[3]+h[3], 
            prev_h[4]+h[4] 
        } : hash_block(
                    Section_T(section,start_index+i+16,scheduled),
                    hash_section(section,h,i),
                    prev_h, start_index, i+16);
    }
  
  public:

    constexpr SHA1(H input) :
        CryptoHash<H,HASH_SIZE>(create_hash(PaddedValue_T(input,true),{})) {}

};


typedef SHA1<> SHA1String;

template<typename T> constexpr uint32_t SHA1<T>::k[4];


#endif

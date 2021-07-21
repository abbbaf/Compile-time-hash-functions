#ifndef COMPILE_TIME_MD5_H
#define COMPILE_TIME_MD5_H

#include "crypto_hash.hpp"

template<typename H=const char *>
class MD5 : public CryptoHash<4> {
  private:

     constexpr static int s[] = { 7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22, 5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20, 4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23, 6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21 };

    constexpr static uint32_t k[] = { 0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee , 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501 , 0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be , 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821 , 0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa , 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8 , 0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed , 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a , 0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c , 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70 , 0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05 , 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665 , 0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039 , 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1 , 0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1 , 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 }; 

    constexpr static int g_array[4][2] = { {1, 0}, {5, 1}, {3, 5}, {7, 0} };


    using Hash_T = MD5<H>;
    using PaddedValue_T = PaddedValue<H>;

    struct hash_parameters {
      const Array<uint32_t,4> arr;
      const uint32_t f_array[4][5];

      template <typename ... Args>
      constexpr hash_parameters(Args ... args):
        arr{args...}, 
        f_array{ {arr[3],0,arr[2],arr[1],arr[3]}, 
                {arr[2],0,arr[1],arr[3],arr[2]}, 
                {arr[3],0,arr[2],0xffffffff,arr[1]}, 
                {~arr[3],arr[1],0,0xffffffff,arr[2]} } {}

      constexpr hash_parameters() : 
        hash_parameters((uint32_t) 0x67452301, (uint32_t) 0xefcdab89, 
                        (uint32_t) 0x98badcfe, (uint32_t) 0x10325476) {}

      constexpr uint32_t operator [](int index) { return arr[index]; }
    };

    constexpr Array<uint32_t,4> create_hash(PaddedValue_T value, hash_parameters h, int block_index=0) {
      return block_index*64 == value.total_size ? Array<uint32_t,4>{
          endianess_convertion(h[0]),
          endianess_convertion(h[1]),
          endianess_convertion(h[2]),
          endianess_convertion(h[3])} :
        create_hash(value, hash_block(value,h,h,block_index), block_index+1);

    }

    constexpr uint32_t f_expr(int i, struct hash_parameters h) const {
      return ((h.f_array[i][0] | h.f_array[i][1]) ^ h.f_array[i][2]) & h.f_array[i][3] ^ h.f_array[i][4];
    }

    constexpr uint32_t f(PaddedValue_T value, int block_index, int i, struct hash_parameters h) const {
      return f_expr(i/16,h) + h[0] + k[i] + value[block_index*16+g(i)];
    }

    constexpr int g(int i) const {
      return (g_array[i/16][0]*i+g_array[i/16][1]) & 0xf;
    }

    constexpr hash_parameters hash_block(PaddedValue_T value, struct hash_parameters h, 
        struct hash_parameters prev_h, int block_index ,int i=0) const { 
      return i == 64 ? 
        (struct hash_parameters) { prev_h[0]+h[0], prev_h[1]+h[1], prev_h[2]+h[2], prev_h[3]+h[3] }
      : hash_block(value, {
            h[3],
            h[1] + leftrotate(f(value, block_index, i, h), s[i]),
            h[1],
            h[2]
        }, prev_h, block_index, i+1);
    }
  
  public:

    constexpr MD5(H input) :
        CryptoHash<4>(create_hash(PaddedValue_T(input,false),{})) {}



};


template<typename T> constexpr int MD5<T>::g_array[4][2];
template<typename T> constexpr int MD5<T>::s[64];
template<typename T> constexpr uint32_t MD5<T>::k[64];

typedef MD5<> MD5String;


#endif

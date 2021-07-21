#ifndef COMPILE_TIME_SHA384_512_H
#define COMPILE_TIME_SHA384_512_H

#include "crypto_hash.hpp"

#define MASK 0xffffffff

template<typename>
class SHA384;

template<typename>
class SHA512;

template<typename>
class SHA512_256;

template<typename>
class SHA512_224;

template<typename H=const char *, int HASH_SIZE>
class SHA384_512 : public CryptoHash<HASH_SIZE> {
  private:

    constexpr static uint64_t ssig0(uint64_t x) {
        return rightrotate64(x,1)^rightrotate64(x,8)^(x >> 7);
    } 

    constexpr static uint64_t ssig1(uint64_t x) {
        return rightrotate64(x,19)^rightrotate64(x,61)^(x >> 6);
    } 

    constexpr static uint64_t scheduled(CircularQueue<uint64_t,16> queue) {
        return ssig1(queue[14]) + queue[9] + ssig0(queue[1]) + queue[0];
    }

   constexpr static uint64_t  k[80] = { 0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc, 0x3956c25bf348b538, 0x59f111f1b605d019, 0x923f82a4af194f9b, 0xab1c5ed5da6d8118, 0xd807aa98a3030242, 0x12835b0145706fbe, 0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2, 0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235, 0xc19bf174cf692694, 0xe49b69c19ef14ad2, 0xefbe4786384f25e3, 0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65, 0x2de92c6f592b0275, 0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5, 0x983e5152ee66dfab, 0xa831c66d2db43210, 0xb00327c898fb213f, 0xbf597fc7beef0ee4, 0xc6e00bf33da88fc2, 0xd5a79147930aa725, 0x06ca6351e003826f, 0x142929670a0e6e70, 0x27b70a8546d22ffc, 0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 0x53380d139d95b3df, 0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6, 0x92722c851482353b, 0xa2bfe8a14cf10364, 0xa81a664bbc423001, 0xc24b8b70d0f89791, 0xc76c51a30654be30, 0xd192e819d6ef5218, 0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8, 0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8, 0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb, 0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3, 0x748f82ee5defb2fc, 0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec, 0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915, 0xc67178f2e372532b, 0xca273eceea26619c, 0xd186b8c721c0c207, 0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178, 0x06f067aa72176fba, 0x0a637dc5a2c898a6, 0x113f9804bef90dae, 0x1b710b35131c471b, 0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc, 0x431d67c49c100d4c, 0x4cc5d4becb3e42b6, 0x597f299cfc657e2a, 0x5fcb6fab3ad6faec, 0x6c44198c4a475817 };


    struct hash_parameters {
        const Array<uint64_t,8> arr;

        template <typename ... Args>
        constexpr hash_parameters(Args ... args): arr{args...} {} 

        constexpr uint64_t operator [](int index) { return arr[index]; }
    };


    using Hash_T = SHA384_512<H,HASH_SIZE>;
    using PaddedValue_T = PaddedValue<H,uint64_t>;
    using Section_T = Section<H,uint64_t>;

    constexpr uint64_t ch(uint64_t x, uint64_t y, uint64_t z) const {
        return (x & y) ^ (~x & z);
    }

    constexpr uint64_t maj(uint64_t x, uint64_t y, uint64_t z) const {
        return (x & y) ^ (x & z) ^ (y & z);
    }

    constexpr uint64_t bsig0(uint64_t x) const {
        return rightrotate64(x,28)^rightrotate64(x,34)^rightrotate64(x,39);
    } 

    constexpr uint64_t bsig1(uint64_t x) const {
        return rightrotate64(x,14)^rightrotate64(x,18)^rightrotate64(x,41);
    }




    constexpr Array<uint32_t,HASH_SIZE> create_hash(PaddedValue_T value, hash_parameters h, int block_index=0) const {
      return block_index*128 == value.total_size 
        ? Array<uint32_t,16,HASH_SIZE>(
               (uint32_t) (h[0] >> 32), (uint32_t) h[0],
               (uint32_t) (h[1] >> 32), (uint32_t) h[1],
               (uint32_t) (h[2] >> 32), (uint32_t) h[2],
               (uint32_t) (h[3] >> 32), (uint32_t) h[3],
               (uint32_t) (h[4] >> 32), (uint32_t) h[4],
               (uint32_t) (h[5] >> 32), (uint32_t) h[5],
               (uint32_t) (h[6] >> 32), (uint32_t) h[6],
               (uint32_t) (h[7] >> 32), (uint32_t) h[7]
            ).truncate()
        : create_hash(
            value, 
            hash_block(Section_T(value,block_index*16,scheduled),h,h,block_index*16),
            block_index+1);
    }

    constexpr uint64_t t1(Section_T section, hash_parameters h, int index) const {
        return h[7] + bsig1(h[4]) + ch(h[4],h[5],h[6]) + k[index] + section[index % 16];  
    }

    constexpr uint64_t t2(hash_parameters h) const {
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
      return i == 80 ? 
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
    constexpr SHA384_512(H input, InitialValues ... initial_values) :
        CryptoHash<HASH_SIZE>(create_hash(PaddedValue_T(input,true),{ initial_values... })) {}

    friend class SHA384<H>;
    friend class SHA512<H>;
    friend class SHA512_256<H>;
    friend class SHA512_224<H>;

};

template<typename T, int HASH_SIZE> constexpr uint64_t SHA384_512<T,HASH_SIZE>::k[80];

#endif

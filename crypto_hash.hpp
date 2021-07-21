#ifndef CRYPTO_HASH_H
#define CRYPTO_HASH_H

#include <iomanip>
#include <cstdint>
#include <iostream>

#include "array.hpp"
#include "utils.hpp"

#include "circular_queue.hpp"
#include "padded_value.hpp"
#include "section.hpp"

//HASH_SIZE = (hash size in bits)/32

template<int HASH_SIZE>
class CryptoHash {


    private:

        const Array<uint32_t,HASH_SIZE> hashed_value;

    protected:

        constexpr CryptoHash(Array<uint32_t,HASH_SIZE> _hashed_value) :
          hashed_value(_hashed_value) {}


    public:

        bool operator ==(const CryptoHash<HASH_SIZE> &second_hash) const {
            for (int i = 0; i < HASH_SIZE; i++) {
                if (hashed_value[i] != second_hash.hashed_value[i]) return false;
            }
            return true;
        }


        bool operator !=(const CryptoHash<HASH_SIZE> &second_hash) const {
            return !(*this == second_hash);
        }

        uint32_t operator [](int index) {
          return hashed_value[index];
        }


        friend std::ostream& operator<<(std::ostream& os, const CryptoHash<HASH_SIZE>& hash) {
            std::ios_base::fmtflags f(std::cout.flags());
            for (int i = 0; i < HASH_SIZE; i++ )
              os << std::hex << std::setfill('0') << std::setw(8) << hash.hashed_value[i]; 
            std::cout.flags(f);
            return os;
        }

};







#endif

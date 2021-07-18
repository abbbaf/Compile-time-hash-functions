#ifndef CRYPTO_HASH_H
#define CRYPTO_HASH_H

#include "utils.hpp"
#include "circular_queue.hpp"
#include "padded_value.hpp"
#include "section.hpp"

using namespace std;

template<typename H, int N>
class CryptoHash {
    private:
        array<uint32_t,N> hashed_value;
        const int size;

    protected:
        constexpr CryptoHash(array<uint32_t,N> _hashed_value, int _size = 0) :
            hashed_value(_hashed_value),
            size(_size ? _size : hashed_value.size()) {}

    public:

        template<typename T, int N2>
        bool operator ==(const CryptoHash<T,N2> &second_hash) const {
          for (int i = 0; i < size; i++) {
            if (hashed_value[i] != second_hash[i]) return false;
          }
          return true;
        }

        template<typename T, int N2>
        bool operator !=(const CryptoHash<T,N2> &second_hash) const {
          return !(this == second_hash);
        }
        
        uint32_t operator[](int index) const {
          return hashed_value[index];
        }

        friend ostream& operator<<(ostream& os, const CryptoHash<H,N>& hash) {
            for (int i = 0; i < hash.size; i++ )
                os << std::hex << setfill('0') << setw(8) << hash[i]; 
            return os;
    }

};

#endif

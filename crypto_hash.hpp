#ifndef CRYPTO_HASH_H
#define CRYPTO_HASH_H

#include "utils.hpp"
#include "circular_queue.hpp"
#include "padded_value.hpp"
#include "section.hpp"

using namespace std;

template<int N, typename Word=uint32_t>
class CryptoHash {
    private:
        static Word W;
        array<Word,N> hashed_value;
        const int size;

    protected:
        constexpr CryptoHash(array<Word,N> _hashed_value, int _size = 0) :
            hashed_value(_hashed_value),
            size(_size ? _size : hashed_value.size()) {}

    public:

        template<typename T, int N2>
        bool operator ==(const CryptoHash<N2,Word> &second_hash) const {
            for (int i = 0; i < size; i++) {
                if (hashed_value[i] != second_hash[i]) return false;
            }
            return true;
        }

        template<typename T, int N2>
        bool operator !=(const CryptoHash<N2,Word> &second_hash) const {
            return !(this == second_hash);
        }
        

        constexpr Word operator[](int index) const {
            return hashed_value[index];
        }

        constexpr size_t length() const {
            return size;
        }

        friend ostream& operator<<(ostream& os, const CryptoHash<N,Word>& hash) {
            ios_base::fmtflags f(cout.flags());
            for (int i = 0; i < hash.size; i++ )
                os << std::hex << setfill('0') << setw(2*sizeof(W)) << hash[i]; 
            cout.flags(f);
            return os;

        }



};




#endif

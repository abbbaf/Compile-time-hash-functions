#ifndef PADDED_VALUE_H
#define PADDED_VALUE_H

template<typename,typename>class Section;

template<typename> class MD5;
template<typename> class SHA1;
template<typename,int> class SHA224_256;
template<typename,int> class SHA384_512;

template<typename H,typename Word=uint32_t>
class PaddedValue {

    constexpr static Word W = 0;

    private:
        const int byte_length = sizeof(W)*16;
        const bool is_big_endian;
        H input;
        const int size, total_size;

        constexpr PaddedValue(H _input, bool _is_big_endian) :
            is_big_endian(_is_big_endian),
            input(_input),
            size(get_size(input)),
            total_size(((size+8) / byte_length + 1)*byte_length) {}


        constexpr unsigned char get_char(int i) const { 
            return i < size ? get_char_from_input<H>(i) : get_padded_byte(i); 
        }



        template<typename F>
        constexpr unsigned char get_char_from_input(int i) const { 
            return get_byte<F>(input,i);
        }

        constexpr unsigned char get_padded_byte(int i) const {
            return (i == size || i < total_size-8) ? 
                0x80*(i==size) : 
                get_byte<uint64_t>(size*8,is_big_endian ? 
                                        (byte_length-i%byte_length-1) : 
                                        (8-byte_length+i%byte_length) );
        }

        constexpr uint32_t get_word_32(int i) const {
            return is_big_endian ?
                   (get_char(4*i) << 3*8) + (get_char(4*i+1) << 2*8) 
                    +(get_char(4*i+2) << 8) + get_char(4*i+3)
                : (get_char(4*i+3) << 3*8) + (get_char(4*i+2) << 2*8)
                  +(get_char(4*i+1) << 8) + get_char(4*i);
        }
        
        constexpr uint64_t get_word_64(int i) const {
            return  ((uint64_t) get_char(8*i) << 7*8) +  ((uint64_t) get_char(8*i+1) << 6*8) 
                    +((uint64_t) get_char(8*i+2) << 5*8) + ((uint64_t) get_char(8*i+3) << 4*8) 
                    +((uint64_t) get_char(8*i+4) << 3*8) + ((uint64_t) get_char(8*i+5) << 2*8) 
                    +((uint64_t) get_char(8*i+6) << 8) + (uint64_t) get_char(8*i+7);
        }



        constexpr Word operator [](int i) const {
            return byte_length == 64 ? get_word_32(i) : get_word_64(i);
        } 

        friend class Section<H,Word>;
        friend class MD5<H>;
        friend class SHA1<H>;
        template<typename,int> friend class SHA224_256;
        template<typename,int> friend class SHA384_512;
};

#endif

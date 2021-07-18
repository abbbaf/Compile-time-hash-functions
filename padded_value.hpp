#ifndef PADDED_VALUE_H
#define PADDED_VALUE_H

template<typename H>
class MD5;

template<typename H>
class SHA1;

template<typename H>
class SHA224_256;

template<typename>
class Section;

template<typename H>
class PaddedValue {

    private:
        H input;
        const size_t size, total_size;
        const bool is_big_endian;

        constexpr PaddedValue(H _input, bool _is_big_endian) :
            is_big_endian(_is_big_endian),
            input(_input),
            size(get_size(input)),
            total_size(((size+8) / 64 + 1)*64) {}

        constexpr unsigned char get_char(unsigned int i) const { 
            return i < size ? get_char_from_input<H>(i) : get_padded_byte(i); 
        }



        template<typename F>
        constexpr unsigned char get_char_from_input(unsigned int i) const { 
            return get_byte<F>(input,i);
        }

        constexpr unsigned char get_padded_byte(unsigned int i) const {
            return (i == size || i < total_size-8) ? 
                0x80*(i==size) : 
                get_byte<uint64_t>(size*8,is_big_endian ? 7-(i%64-56) : (i%64-56));
        }

        constexpr uint32_t operator [](int i) const {
            return is_big_endian ?
                   (get_char(4*i) << 3*8) + (get_char(4*i+1) << 2*8) 
                    +(get_char(4*i+2) << 8) + get_char(4*i+3)
                : (get_char(4*i+3) << 3*8) + (get_char(4*i+2) << 2*8)
                  +(get_char(4*i+1) << 8) + get_char(4*i);
        } 

        friend class MD5<H>;
        friend class SHA1<H>;
        friend class SHA224_256<H>;
        friend class Section<H>;

};

#endif

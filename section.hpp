#ifndef SECTION_H
#define SECTION_H

template<typename>
class SHA1;

template<typename>
class SHA224;

template<typename>
class SHA224_256;

template<typename H>
class SHA384_512;

template<typename H, typename Word=uint32_t>
class Section {

    typedef CircularQueue<Word,16> CircularQueueInts;
    typedef Word (*scheduled_func)(CircularQueueInts);
    const int start_index; 
    const int block_start_index;
    const PaddedValue<H,Word> main_value;
    const CircularQueueInts queue;
    

    constexpr Section(PaddedValue<H,Word> value, int _start_index, scheduled_func scheduled) :
        start_index(_start_index),
        block_start_index(_start_index),
        main_value(value),
        queue(fill_next_ints(CircularQueueInts(),scheduled)) {}


    constexpr Section(const Section &prev_section, int _start_index, scheduled_func scheduled) :
        start_index(_start_index),
        block_start_index(prev_section.block_start_index),
        main_value(prev_section.main_value),
        queue(fill_next_ints(prev_section.queue,scheduled)) {}


    constexpr CircularQueueInts fill_next_ints(CircularQueueInts queue, scheduled_func scheduled, 
                                            int i=0) const {
        return i == 16 ? queue 
              : fill_next_ints(
                   CircularQueueInts(
                     get_int(queue, start_index+i, scheduled)
                     ,queue)
               ,scheduled,i+1);
    }

    constexpr Word get_int(CircularQueueInts queue, int index, scheduled_func scheduled) const {
        return index - block_start_index < 16 ? 
              main_value[index]
            : scheduled(queue); 
    }

    constexpr Word operator[](int index) const {
        return queue[index];
    }

    friend class SHA1<H>;
    friend class SHA224<H>;
    friend class SHA224_256<H>;
    friend class SHA384_512<H>;

};

#endif

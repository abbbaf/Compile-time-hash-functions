#ifndef SECTION_H
#define SECTION_H


using CircularQueueInts = CircularQueue<uint32_t,16>; 
typedef uint32_t (*scheduled_func)(CircularQueueInts);


template<typename>
class SHA1;

template<typename>
class SHA224;

template<typename>
class SHA224_256;

template<typename H>
class Section {

      const int start_index; 
      const int block_start_index;
      const PaddedValue<H> main_value;
      const CircularQueueInts queue;
    

      constexpr Section(PaddedValue<H> value, int _start_index, scheduled_func scheduled) :
        main_value(value),
        start_index(_start_index),
        block_start_index(_start_index),
        queue(fill_next_ints(CircularQueueInts(),scheduled)) {}


      constexpr Section(const Section &prev_section, int _start_index, scheduled_func scheduled) :
        main_value(prev_section.main_value),
        start_index(_start_index),
        block_start_index(prev_section.block_start_index),
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

      //Retrieve a 32 bit integer
      constexpr uint32_t get_int(CircularQueueInts queue, int index, scheduled_func scheduled) const {
        return index - block_start_index < 16 ? 
              main_value[index]
            : scheduled(queue); 
      }

      constexpr uint32_t operator[](int index) const {
        return queue[index];
      }

      friend class SHA1<H>;
      friend class SHA224<H>;
      friend class SHA224_256<H>;

};

#endif

#ifndef CIRCULAR_QUEUE
#define CIRCULAR_QUEUE

#include <array>

using namespace std;

template<typename T, size_t N, int Counter=N>
class CircularQueue {
  public:
    const array<T,N> arr;

    template<typename ... Args>
    constexpr CircularQueue(int new_item, CircularQueue<T,N> queue, Args ... args) :
      arr(CircularQueue<T,N,Counter-1>(new_item, queue,args...,queue[N - Counter]).arr) {}

    constexpr CircularQueue(array<T,N> _arr) :
      arr{_arr} {}

    constexpr CircularQueue() :
      arr{0} {}


    constexpr T operator[](int index) const {
      return arr[index];
    }

    constexpr size_t size() const {
      return N;
    }

};

template<typename T, size_t N>
class CircularQueue<T,N,0> {

  public:
    array<T,N> arr; 

    template<typename ... Args>
    constexpr CircularQueue(T new_item, CircularQueue<T,N> queue, Args ... args) :
      arr{add_item(new_item,args...)} {}  

    template<typename ... Args>
    constexpr array<T,N> add_item(T new_item, T first, Args... args) const {
      return array<T,N>{args..., new_item };
    }

};

#endif

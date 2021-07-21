#ifndef ARRAY2_H
#define ARRAY2_H

template<typename T, int N, int TRUNCATED_SIZE=N, int Counter=N>
class Array {
  
  private:
    const T arr[N];

  public:

    template<typename ... Args>
    constexpr Array(T first, Args ... args) : arr{first, args...} {}

    constexpr Array() : arr{ 0 } {}

    constexpr T operator[](int index) const {
      return arr[index];
    }

    constexpr int size() const {
      return N;
    }

    constexpr Array<T,TRUNCATED_SIZE> truncate() const {
      return Array<T,TRUNCATED_SIZE>().__truncate(&arr[0]);
    }

    template<typename ... Elements>
    constexpr Array<T,N> __truncate(const T * source, Elements ... elements) const {
      return Array<T,N,TRUNCATED_SIZE,Counter-1>().__truncate(source,elements...,source[N-Counter]);
    }
}; 

template<typename T, int N, int TRUNCATED_SIZE>
class Array<T,N,TRUNCATED_SIZE,0> {

  public:
    template<typename ... Elements>
    constexpr Array<T,N> __truncate(const T* source, Elements ... elements) {
      return Array<T,N>{ elements... };
  }

};



#endif

# Compile time hash functions

## Description
This is an implementation for md5, sha1 and sha2 (all version).
It can be used to generate a hash at compile time, such that the binary file
will have the hash but not the original string.
It can also be used at runtime.

## Supports C++11/14/17/20 

## Classes
In all classes the type T should be either const char * or a primitive type.
std:string can be used at runtime only (for c++ versions below 20).
For any other type it will just hash the value of the pointer.

### C++11/14
```
* MD5<T>(T value)
* SHA1<T>(T value)
* SHA224<T>(T value)  
* SHA256<T>(T value)
* SHA384<T>(T value)
* SHA512<T>(T value)
* SHA512_224<T>(T value)
* SHA512_256<T>(T value)
```
 
#### If T is const char * (string literal) than the following classes can also be used:
 ``` 
* MD5String(T value)
* SHA1String(T value)
* SHA224String(T value)  
* SHA256String(T value)
* SHA384String(T value)
* SHA512String(T value)
* SHA512_224String(T value)
* SHA512_256String(T value)
  ```

 ###  C++17/20
  Same as older versions but without the T.
  e.g. MD5(T value)
  
 ## Operators supported
  ==, !=, [],  << (ostream)
  
  The subscript operator treats the hash as an array of 32 bit integers
  
 ## Example
  
 ```#include <cstdlib>
  #include <iostream>
 
  #include "compile_time_hash.hpp"
  
  using namespace std;
  
  //constexpr must be used here
  constexpr SHA256 hash_value = SHA256("Hello World");
  
  int main() {
    cout << hash_value << endl;
  }
  ```
  prints: a591a6d40bf420404a011733cfb7b190d62c65bf0bcda32b57b277d9ad9f146e

  The string "Hello World" will not appear in the final binary.

## Usage with CMake

If you want to use CMake in your project, you can include this library in two ways:

### As a submodule

1. Add this repository as a git submodule:
   `git submodule add https://github.com/abbbaf/Compile-time-hash-functions.git`
1. Include this code in your CMakeLists.txt:
   ```cmake
   add_subdirectory(Compile-time-hash-functions)
   target_link_libraries(YOUR_TARGET compile_time_hash)
   ```
1. Include it in your `.cpp` file:
   ```cpp
   #include <compile_time_hash.hpp>
   ```
### Install it globally

1. Clone this repository somewhere:
   `git clone https://github.com/abbbaf/Compile-time-hash-functions.git`
1. Install it into the global include directory:
1. Add this to your CMakeLists.txt:
   ```cmake
   find_package(compile_time_hash REQUIRED)
   target_link_libraries(CTH_test_2 compile_time_hash::compile_time_hash)
    ```
1. Include it in your `.cpp` file:
   ```cpp
   #include <compile_time_hash/compile_time_md5.hpp>
   ```

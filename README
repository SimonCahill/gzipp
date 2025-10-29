# gzipp

A tiny, header-only gzip helper for modern C++ (C++20). It wraps zlib to provide simple functions to compress, decompress, and detect compressed data with std::string and raw/binary buffers.

> This library is inspired by mapbox/gzip-hpp but is a separate project with its own API.

## Features

- Header-only: just include and link zlib
- C++20 concepts for flexible inputs (strings and binary buffers)
- Simple API: `compress`, `decompress`, `isCompressed`
- Overloads for `std::string`, `std::string_view`, raw pointers and `uint8_t` buffers
- Supports both gzip and zlib-encoded payloads for detection
- Sensible safety limits (default 2 GiB max input)

## Requirements

- C++20 compiler (tested with recent GCC/Clang)
- zlib development package (e.g., `zlib1g-dev` on Debian/Ubuntu)
- CMake ≥ 3.10 to consume via CMake

## Getting started

Include the umbrella header and link against zlib and the `gzipp::gzipp` interface target if you use CMake.

### Quick example

```cpp
#include <gzipp/gzipp.hpp>
#include <iostream>
#include <string>

int main() {
    const std::string input = "Hello, World!";

    // Compress with default level (Z_DEFAULT_COMPRESSION)
    std::string compressed = gzipp::compress(input);

    // Check if the buffer looks compressed (gzip or zlib magic)
    if (gzipp::isCompressed(compressed)) {
        std::string plain = gzipp::decompress(compressed);
        std::cout << plain << "\n"; // prints: Hello, World!
    }
}
```

### Build a tiny example with g++

```bash
# from the repository root
cat > hello.cpp <<'CPP'
#include <gzipp/gzipp.hpp>
#include <iostream>
int main(){
  std::string s = "Hello, World!";
  auto c = gzipp::compress(s);
  auto d = gzipp::decompress(c);
  std::cout << d << "\n";
}
CPP

g++ -std=c++20 -Iinclude hello.cpp -lz -o hello
./hello
```

## Using with CMake

This repository defines a header-only interface target named `gzipp` with the aliases `gzipp::gzipp` and `procsys::gzipp`.

Add the project and link it:

```cmake
# CMakeLists.txt of your app/library
cmake_minimum_required(VERSION 3.10)
project(my_app LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

# Option A: add the local checkout
add_subdirectory(${CMAKE_SOURCE_DIR}/external/gzipp ${CMAKE_BINARY_DIR}/_gzipp)

add_executable(my_app main.cpp)

target_link_libraries(my_app PRIVATE gzipp::gzipp) # zlib is propagated transitively
```

If you prefer FetchContent, you can adapt this pattern (replace URL/TAG with your source):

```cmake
include(FetchContent)
FetchContent_Declare(
  gzipp
  GIT_REPOSITORY https://example.com/owner/gzipp.git # replace with the actual URL
  GIT_TAG        main                                 # or a version tag/commit
)
FetchContent_MakeAvailable(gzipp)

add_executable(my_app main.cpp)

target_link_libraries(my_app PRIVATE gzipp::gzipp)
```

## API overview

Include header: `#include <gzipp/gzipp.hpp>`

- `std::string compress(const T& input, int level = Z_DEFAULT_COMPRESSION, size_t maxSize = gzipp::DEFAULT_MAX_COMPRESSABLE_SIZE)`
  - T must satisfy `Gzipable` (string-like or binary buffer with `.data()` and `.size()`).
- `std::string compress(const char* input, size_t size, int level = Z_DEFAULT_COMPRESSION)`
- `std::string compress(const uint8_t* input, size_t size, int level = Z_DEFAULT_COMPRESSION)`

- `std::string decompress(const T& input)`
- `std::string decompress(const char* input, size_t size)`
- `std::string decompress(const uint8_t* input, size_t size)`

- `bool isCompressed(const T& input)`
- `bool isCompressed(const char* input, size_t size)`
- `bool isCompressed(const uint8_t* input, size_t size)`

Config:

- `constexpr size_t gzipp::DEFAULT_MAX_COMPRESSABLE_SIZE = 2_GiB;`

### Examples

Compress/decompress a string view with custom level:

```cpp
std::string_view view = "some text";
auto compressed = gzipp::compress(view, Z_BEST_SPEED);
auto plain      = gzipp::decompress(compressed);
```

Compress raw bytes (e.g., vector<uint8_t>):

```cpp
std::vector<uint8_t> bytes = {1,2,3,4,5};
auto compressed = gzipp::compress(bytes);
```

Raw pointer + size:

```cpp
const uint8_t* data = /*...*/;
size_t len = /*...*/;
auto compressed = gzipp::compress(data, len);
auto plain      = gzipp::decompress(compressed);
```

Check whether data is compressed:

```cpp
if (gzipp::isCompressed(buffer)) {
  auto plain = gzipp::decompress(buffer);
}
```

## Error handling and limits

- `compress` throws `std::runtime_error` if zlib initialization/finalization fails or when input exceeds `maxSize` (default 2 GiB).
- `decompress` throws `std::runtime_error` on decode errors or if the input exceeds internal safety limits.
- `isCompressed` checks common gzip/zlib magic bytes and returns `false` for empty inputs.

Consider wrapping calls in try/catch if processing untrusted or potentially malformed data.

## Building and running tests (optional)

This repo contains GoogleTest-based unit tests.

```bash
# Configure with tests enabled
cmake -S . -B build -Dgzipp_BUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
ctest --test-dir build --output-on-failure
```

You need a GTest installation discoverable by CMake (e.g., `libgtest-dev` on Debian/Ubuntu).

## License

BSD-2-Clause. See `LICENSE`.

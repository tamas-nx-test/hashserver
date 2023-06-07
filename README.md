# hss

## Building

### Prerequisites

- Conan 2.x
- gcc 11+

### Steps to build

On both Linux and Windows, use the following steps to build this project:

```sh
$ conan profile detect --force # optional
$ conan install . --output-folder=build --build=missing
$ cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake -G Ninja
$ cmake --build build
```

To run the unit tests:

```sh
$ cd build
$ ctest .
```

### Usage

To run the server, start `./build/src/hss` (or `<prefix>/bin/hss` if you built the install target)

To terminate the server, press Ctrl+C.

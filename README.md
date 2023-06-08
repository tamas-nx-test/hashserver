# hss

Please find my detailed notes in [NOTES.md](NOTES.md) about my thought process.

## Building

### Prerequisites

- Conan 1.x
- gcc 11+ or Visual Studio 2019

### Steps to build

On both Linux and Windows, use the following steps to build this project:

```sh
$ mkdir build && cd build
$ conan install .. -pr ../profiles/<os>/x86_64_release --build=missing  # <os> is either linux or windows
$ cmake .. -DCMAKE_TOOLCHAIN_FILE="conan_paths.cmake" -DCMAKE_BUILD_TYPE=Release -G Ninja -DCMAKE_INSTALL_PREFIX=dist
$ ninja install
```

To run the unit tests:

```sh
$ cd build
$ ctest .
```

To run the integration tests, start the server (see below in the Usage section) and execute the
`integration_tests.py` script.

### Usage

To run the server, start `./dist/bin/hss` after building the install target. Note, you could run
`./src/hss`, but on Windows this will fail because there is no notion of RPATHs on this platform
and serverlib.dll needs to be in the same directory. The install target ensure that this is the
case.

Connect the host running the server on port 8001.

To terminate the server, press Ctrl+C.

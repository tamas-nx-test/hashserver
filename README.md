# hss

## Building

```sh
$ cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=<vcpkg>/scripts/buildsystems/vcpkg.cmake -G Ninja -DCMAKE_INSTALL_PREFIX=$(pwd)/build/dist
```
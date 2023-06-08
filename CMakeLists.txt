cmake_minimum_required(VERSION 3.20)
project(hashserver VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/")
include(Dependencies)

add_subdirectory(src)
add_subdirectory(lib)

set(CPACK_GENERATOR ZIP)
include(CPack)

enable_testing()
add_subdirectory(test)

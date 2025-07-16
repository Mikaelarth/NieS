cmake_minimum_required(VERSION 3.5)
project(NieS LANGUAGES CXX)

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)

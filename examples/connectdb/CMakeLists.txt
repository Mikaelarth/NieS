cmake_minimum_required(VERSION 3.5)
project(connectdb LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Sql REQUIRED)

add_executable(connectdb main.cpp)

target_link_libraries(connectdb Qt5::Sql)

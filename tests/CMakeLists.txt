cmake_minimum_required(VERSION 3.5)

set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Test Sql REQUIRED)

set(TEST_SOURCES
    database_test.cpp
    ${CMAKE_SOURCE_DIR}/src/DatabaseManager.cpp
)

add_executable(nies_tests ${TEST_SOURCES})

target_include_directories(nies_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(nies_tests Qt5::Test Qt5::Sql)

add_test(NAME NieSTests COMMAND nies_tests)

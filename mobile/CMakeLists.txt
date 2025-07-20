cmake_minimum_required(VERSION 3.5)
project(NieSMobile LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Quick Network REQUIRED)

add_executable(NieSMobile
    main.cpp
    RestClient.cpp
    qml.qrc
)

target_include_directories(NieSMobile PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(NieSMobile Qt5::Quick Qt5::Network)

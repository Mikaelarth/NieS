cmake_minimum_required(VERSION 3.5)
project(NieSApp LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets Sql REQUIRED)

add_executable(NieSApp
    main.cpp
    DatabaseManager.cpp
    UserManager.cpp
    ProductManager.cpp
    InventoryManager.cpp
    SalesManager.cpp
)

target_link_libraries(NieSApp Qt5::Widgets Qt5::Sql)

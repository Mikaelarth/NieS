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
    products/ProductWindow.cpp
    InventoryManager.cpp
    SalesManager.cpp
    login/LoginDialog.cpp
    login/MainWindow.cpp
)

target_link_libraries(NieSApp Qt5::Widgets Qt5::Sql)
target_include_directories(NieSApp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

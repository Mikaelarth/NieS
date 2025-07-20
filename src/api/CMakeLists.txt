cmake_minimum_required(VERSION 3.5)
project(NieSApi LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core Network Sql REQUIRED)

add_executable(NieSApi
    main.cpp
    RestServer.cpp
    ../DatabaseManager.cpp
    ../ProductManager.cpp
    ../InventoryManager.cpp
    ../SalesManager.cpp
    ../UserSession.cpp
)

target_include_directories(NieSApi PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(NieSApi Qt5::Core Qt5::Network Qt5::Sql)

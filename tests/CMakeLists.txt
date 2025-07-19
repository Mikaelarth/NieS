cmake_minimum_required(VERSION 3.5)

set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Test Sql Widgets REQUIRED)

set(TEST_SOURCES
    database_test.cpp
    login_test.cpp
    product_window_test.cpp
    pos_window_test.cpp
    sales_report_window_test.cpp
    main_window_test.cpp
    ${CMAKE_SOURCE_DIR}/src/DatabaseManager.cpp
    ${CMAKE_SOURCE_DIR}/src/UserManager.cpp
    ${CMAKE_SOURCE_DIR}/src/ProductManager.cpp
    ${CMAKE_SOURCE_DIR}/src/products/ProductWindow.cpp
    ${CMAKE_SOURCE_DIR}/src/sales/POSWindow.cpp
    ${CMAKE_SOURCE_DIR}/src/sales/SalesReportWindow.cpp
    ${CMAKE_SOURCE_DIR}/src/InventoryManager.cpp
    ${CMAKE_SOURCE_DIR}/src/SalesManager.cpp
    ${CMAKE_SOURCE_DIR}/src/login/LoginDialog.cpp
    ${CMAKE_SOURCE_DIR}/src/login/MainWindow.cpp
)

add_executable(nies_tests ${TEST_SOURCES})

target_include_directories(nies_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(nies_tests Qt5::Test Qt5::Sql Qt5::Widgets)

add_test(NAME NieSTests COMMAND nies_tests)

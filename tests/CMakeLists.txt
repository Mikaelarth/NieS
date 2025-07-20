cmake_minimum_required(VERSION 3.5)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Test Sql Widgets Network NetworkInformation Charts)
if(Qt6_FOUND)
    set(QT_PACKAGE Qt6)
else()
    find_package(Qt5 COMPONENTS Test Sql Widgets Network Charts REQUIRED)
    set(QT_PACKAGE Qt5)
endif()

set(TEST_SOURCES
    database_test.cpp
    login_test.cpp
    user_window_test.cpp
    product_window_test.cpp
    pos_window_test.cpp
    sales_report_window_test.cpp
    main_window_test.cpp
    return_manager_test.cpp
    invoice_printer_test.cpp
    payment_processor_test.cpp
    rest_server_test.cpp
    dashboard_window_test.cpp
    stock_prediction_test.cpp
    loyalty_manager_test.cpp
    employee_manager_test.cpp
    employee_window_test.cpp
    inventory_window_test.cpp
    barcode_scanner_test.cpp
    config_option_test.cpp
    ${CMAKE_SOURCE_DIR}/src/DatabaseManager.cpp
    ${CMAKE_SOURCE_DIR}/src/UserManager.cpp
    ${CMAKE_SOURCE_DIR}/src/ProductManager.cpp
    ${CMAKE_SOURCE_DIR}/src/products/ProductWindow.cpp
    ${CMAKE_SOURCE_DIR}/src/sales/POSWindow.cpp
    ${CMAKE_SOURCE_DIR}/src/inventory/InventoryWindow.cpp
    ${CMAKE_SOURCE_DIR}/src/sales/SalesReportWindow.cpp
    ${CMAKE_SOURCE_DIR}/src/InventoryManager.cpp
    ${CMAKE_SOURCE_DIR}/src/SalesManager.cpp
    ${CMAKE_SOURCE_DIR}/src/returns/ReturnManager.cpp
    ${CMAKE_SOURCE_DIR}/src/invoices/InvoicePrinter.cpp
    ${CMAKE_SOURCE_DIR}/src/payments/PaymentProcessor.cpp
    ${CMAKE_SOURCE_DIR}/src/stock/StockPrediction.cpp
    ${CMAKE_SOURCE_DIR}/src/stock/StockPredictionWindow.cpp
    ${CMAKE_SOURCE_DIR}/src/dashboard/DashboardWindow.cpp
    ${CMAKE_SOURCE_DIR}/src/loyalty/LoyaltyManager.cpp
    ${CMAKE_SOURCE_DIR}/src/loyalty/LoyaltyWindow.cpp
    ${CMAKE_SOURCE_DIR}/src/login/LoginDialog.cpp
    ${CMAKE_SOURCE_DIR}/src/login/MainWindow.cpp
    ${CMAKE_SOURCE_DIR}/src/login/UserWindow.cpp
    ${CMAKE_SOURCE_DIR}/src/employees/EmployeeWindow.cpp
    ${CMAKE_SOURCE_DIR}/src/api/RestServer.cpp
    ${CMAKE_SOURCE_DIR}/src/UserSession.cpp
    ${CMAKE_SOURCE_DIR}/src/employees/EmployeeManager.cpp
    ${CMAKE_SOURCE_DIR}/src/barcode/BarcodeScanner.cpp
)

add_executable(nies_tests ${TEST_SOURCES})

target_include_directories(nies_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(nies_tests ${QT_PACKAGE}::Test ${QT_PACKAGE}::Sql ${QT_PACKAGE}::Widgets ${QT_PACKAGE}::Network ${QT_PACKAGE}::Charts)
if(Qt6_FOUND)
    target_link_libraries(nies_tests Qt6::NetworkInformation)
endif()

add_test(NAME NieSTests COMMAND nies_tests)

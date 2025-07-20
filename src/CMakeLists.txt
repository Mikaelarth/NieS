cmake_minimum_required(VERSION 3.5)
project(NieSApp LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets Sql Network LinguistTools Charts REQUIRED)

add_executable(NieSApp
    main.cpp
    DatabaseManager.cpp
    UserManager.cpp
    ProductManager.cpp
    products/ProductWindow.cpp
    InventoryManager.cpp
    SalesManager.cpp
    returns/ReturnManager.cpp
    invoices/InvoicePrinter.cpp
    payments/PaymentProcessor.cpp
    stock/StockPrediction.cpp
    stock/StockPredictionWindow.cpp
    sales/POSWindow.cpp
    sales/SalesReportWindow.cpp
    loyalty/LoyaltyManager.cpp
    loyalty/LoyaltyWindow.cpp
    dashboard/DashboardWindow.cpp
    login/LoginDialog.cpp
    login/MainWindow.cpp
    login/UserWindow.cpp
    NetworkMonitor.cpp
    UserSession.cpp
    employees/EmployeeManager.cpp
    barcode/BarcodeScanner.cpp
)

target_link_libraries(NieSApp Qt5::Widgets Qt5::Sql Qt5::Network Qt5::Charts)
target_include_directories(NieSApp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

set(TS_FILES
    ${CMAKE_SOURCE_DIR}/translations/NieS_fr.ts
)
qt5_add_translation(QM_FILES ${TS_FILES})
add_custom_target(translations ALL DEPENDS ${QM_FILES})

add_custom_command(TARGET NieSApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:NieSApp>/translations
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${QM_FILES} $<TARGET_FILE_DIR:NieSApp>/translations
)

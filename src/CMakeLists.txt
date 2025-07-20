cmake_minimum_required(VERSION 3.5)
project(NieSApp LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Widgets Sql Network NetworkInformation LinguistTools Charts)
if(Qt6_FOUND)
    set(QT_PACKAGE Qt6)
else()
    find_package(Qt5 COMPONENTS Widgets Sql Network LinguistTools Charts REQUIRED)
    set(QT_PACKAGE Qt5)
endif()

add_executable(NieSApp
    main.cpp
    DatabaseManager.cpp
    UserManager.cpp
    ProductManager.cpp
    products/ProductWindow.cpp
    inventory/InventoryWindow.cpp
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
    employees/EmployeeWindow.cpp
    NetworkMonitor.cpp
    UserSession.cpp
    employees/EmployeeManager.cpp
    barcode/BarcodeScanner.cpp
)

target_link_libraries(NieSApp ${QT_PACKAGE}::Widgets ${QT_PACKAGE}::Sql ${QT_PACKAGE}::Network ${QT_PACKAGE}::Charts)
if(Qt6_FOUND)
    target_link_libraries(NieSApp Qt6::NetworkInformation)
endif()
target_include_directories(NieSApp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

set(TS_FILES
    ${CMAKE_SOURCE_DIR}/translations/NieS_fr.ts
)
if(Qt6_FOUND)
    qt6_add_translation(QM_FILES ${TS_FILES})
else()
    qt5_add_translation(QM_FILES ${TS_FILES})
endif()
add_custom_target(translations ALL DEPENDS ${QM_FILES})

add_custom_command(TARGET NieSApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:NieSApp>/translations
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${QM_FILES} $<TARGET_FILE_DIR:NieSApp>/translations
)

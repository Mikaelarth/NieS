QT += widgets sql network charts
qtHaveModule(networkinformation): QT += networkinformation
CONFIG += c++11 console
TEMPLATE = app
TARGET = NieSApp
INCLUDEPATH += $$PWD/src

SOURCES += \
    src/main.cpp \
    src/DatabaseManager.cpp \
    src/UserManager.cpp \
    src/ProductManager.cpp \
    src/InventoryManager.cpp \
    src/SalesManager.cpp \
    src/loyalty/LoyaltyManager.cpp \
    src/loyalty/LoyaltyWindow.cpp \
    src/UserSession.cpp \
    src/login/LoginDialog.cpp \
    src/login/MainWindow.cpp \
    src/NetworkMonitor.cpp \
    src/products/ProductWindow.cpp \
    src/inventory/InventoryWindow.cpp \
    src/returns/ReturnManager.cpp \
    src/invoices/InvoicePrinter.cpp \
    src/payments/PaymentProcessor.cpp \
    src/stock/StockPrediction.cpp \
    src/stock/StockPredictionWindow.cpp \
    src/sales/POSWindow.cpp \
    src/sales/SalesReportWindow.cpp \
    src/dashboard/DashboardWindow.cpp \
    src/login/UserWindow.cpp \
    src/employees/EmployeeWindow.cpp \
    src/employees/EmployeeManager.cpp \
    src/barcode/BarcodeScanner.cpp

HEADERS += \
    src/DatabaseManager.h \
    src/UserManager.h \
    src/ProductManager.h \
    src/InventoryManager.h \
    src/SalesManager.h \
    src/loyalty/LoyaltyManager.h \
    src/loyalty/LoyaltyWindow.h \
    src/UserSession.h \
    src/login/LoginDialog.h \
    src/login/MainWindow.h\
    src/NetworkMonitor.h \
    src/products/ProductWindow.h \
    src/inventory/InventoryWindow.h \
    src/returns/ReturnManager.h \
    src/invoices/InvoicePrinter.h \
    src/payments/PaymentProcessor.h \
    src/stock/StockPrediction.h \
    src/stock/StockPredictionWindow.h \
    src/sales/POSWindow.h \
    src/sales/SalesReportWindow.h \
    src/dashboard/DashboardWindow.h \
    src/login/UserWindow.h \
    src/employees/EmployeeWindow.h \
    src/employees/EmployeeManager.h \
    src/barcode/BarcodeScanner.h

# Include config file for convenience
OTHER_FILES += config.ini

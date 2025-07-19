QT += widgets sql network
CONFIG += c++11 console
TEMPLATE = app
TARGET = NieSApp

SOURCES += \
    src/main.cpp \
    src/DatabaseManager.cpp \
    src/UserManager.cpp \
    src/ProductManager.cpp \
    src/InventoryManager.cpp \
    src/SalesManager.cpp \
    src/UserSession.cpp \
    src/login/LoginDialog.cpp \
    src/login/MainWindow.cpp
    src/NetworkMonitor.cpp

HEADERS += \
    src/DatabaseManager.h \
    src/UserManager.h \
    src/ProductManager.h \
    src/InventoryManager.h \
    src/SalesManager.h \
    src/UserSession.h \
    src/login/LoginDialog.h \
    src/login/MainWindow.h
    src/NetworkMonitor.h

# Include config file for convenience
OTHER_FILES += config.ini

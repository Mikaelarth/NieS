QT += widgets sql
CONFIG += c++11 console
TEMPLATE = app
TARGET = NieSApp

SOURCES += \
    src/main.cpp \
    src/DatabaseManager.cpp \
    src/UserManager.cpp \
    src/ProductManager.cpp \
    src/InventoryManager.cpp \
    src/SalesManager.cpp

HEADERS += \
    src/DatabaseManager.h \
    src/UserManager.h \
    src/ProductManager.h \
    src/InventoryManager.h \
    src/SalesManager.h

# Include config file for convenience
OTHER_FILES += config.ini

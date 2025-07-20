QT += widgets sql network charts
qtHaveModule(networkinformation): QT += networkinformation
CONFIG += c++11 console
TEMPLATE = app
TARGET = NieSApp
INCLUDEPATH += $$PWD/src

# Automatically include all C++ source files found under src/
# This uses the qmake files() function with recursive search to avoid
# manually updating the project file whenever a new file is added.
SOURCES += $$files($$PWD/src/*.cpp, true)

# Collect header files recursively as well
HEADERS += $$files($$PWD/src/*.h, true)

# Include config file for convenience
OTHER_FILES += config.ini

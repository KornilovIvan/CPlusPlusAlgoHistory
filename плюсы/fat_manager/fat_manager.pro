QT += core gui widgets

CONFIG += c++17
TEMPLATE = app
TARGET = fat_manager

SOURCES += \
    src/main.cpp \
    src/MainWindow.cpp \
    src/LeftPanel.cpp \
    src/RightPanel.cpp \
    src/FatParser.cpp 

HEADERS += \
    include//MainWindow.h \
    include/LeftPanel.h \
    include/RightPanel.h \
    include/FatStructures.h \
    include/FatParser.h 

DESTDIR = bin
OBJECTS_DIR = build/obj
MOC_DIR = build/moc

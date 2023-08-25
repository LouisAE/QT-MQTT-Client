QT += core gui
QT += widgets
QT += network
TEMPLATE = app
TARGET = QtMqtt
DESTDIR = build/bin

MOC_DIR += build/moc
OBJECTS_DIR += build/o
UI_DIR += build/ui
RCC_DIR += build/rcc
include(QtMqtt.pri)

win32-msvc* {
    LIBS += -L$$PWD/lib/msvc -lQt5Qmqtt
    QMAKE_CXXFLAGS += /source-charset:utf-8 /execution-charset:utf-8
}

win32:mingw{
    LIBS += -L$$PWD/lib/mingw -llibQt5Qmqtt
}

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

QT += sql core gui widgets

CONFIG += c++2a

DEFINES += QT_DEPRECATED_WARNINGS \
    SQLITE_AUTO_TESTS

TEMPLATE = app

SOURCES += tst_autotest.cpp \
    core/sqlite.cpp

HEADERS += core/sqlite.h

#-------------------------------------------------
#
# Project created by QtCreator 2016-12-29T14:07:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FrontsTest
TEMPLATE = app


SOURCES += main.cpp\
        mw.cpp \
    font2c.cpp \
    font2cindex.cpp \
    font2cglyph.cpp \
    fontselect.cpp \
    testjson.cpp

HEADERS  += mw.h \
    font2c.h \
    font2cindex.h \
    font2cglyph.h \
    fontselect.h \
    testjson.h

FORMS    += mw.ui \
    fontselect.ui

RESOURCES += \
    res.qrc


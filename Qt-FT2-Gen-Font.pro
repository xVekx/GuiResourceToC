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
    genglyph.cpp \
    genindex.cpp \
    genfont.cpp

HEADERS  += mw.h \
    genglyph.h \
    genindex.h \
    genfont.h

FORMS    += mw.ui

LIBS += -lz -lfreetype

INCLUDEPATH += $$MOC_DIR \
               /usr/include \
               /usr/include/freetype2


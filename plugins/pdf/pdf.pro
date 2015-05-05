#-------------------------------------------------
#
# Project created by QtCreator 2015-04-08T16:13:24
#
#-------------------------------------------------




TEMPLATE = lib
CONFIG += plugin
QT       += core gui
QMAKE_CXXFLAGS += -std=c++11
LIBS += -L/usr/local/lib -lpoppler-qt4
SOURCES += pdf_plugin.cpp\
          ../../Base/code/attribute.cpp\
          ../../Base/code/file.cc\
          ../../Base/code/treebranch.cpp

HEADERS  += pdf_plugin.h\
          ../../Base/code/attribute.h\
          ../../Base/poppler/src/poppler-qt4.h\
          ../../Base/code/file.h\
          ../../Base/code/treebranch.h

INCLUDEPATH += ../../Base
DESTDIR = ../../Base/plugins
TARGET = $$qtLibraryTarget(pdf_plugin)



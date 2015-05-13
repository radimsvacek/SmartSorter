#-------------------------------------------------
#
# Project created by QtCreator 2015-04-08T16:13:24
#
#-------------------------------------------------




TEMPLATE = lib
CONFIG += plugin
QT       += core gui\
            webkit
QMAKE_CXXFLAGS += -std=c++11

SOURCES += html_plugin.cpp\
          ../../Base/code/attribute.cpp\
          ../../Base/code/file.cc\
          ../../Base/code/treebranch.cpp

HEADERS  += html_plugin.h\
          ../../Base/code/attribute.h\
          ../../Base/code/file.h\
          ../../Base/code/treebranch.h

INCLUDEPATH += ../../Base
DESTDIR = ../../Base/plugins
TARGET = $$qtLibraryTarget(html_plugin)


target.path = /usr/local/bin/plugins
INSTALLS += target



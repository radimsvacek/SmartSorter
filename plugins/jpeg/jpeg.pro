#-------------------------------------------------
#
# Project created by QtCreator 2015-04-08T16:13:24
#
#-------------------------------------------------


TEMPLATE = lib
CONFIG += plugin
QT       += core gui
QMAKE_CXXFLAGS += -std=c++11
LIBS         += -lexif
SOURCES += jpeg_plugin.cpp\
          ../../Base/code/attribute.cpp\
          ../../Base/code/file.cc\
          ../../Base/code/treebranch.cpp

HEADERS  += jpeg_plugin.h\
          ../../Base/code/attribute.h\
          ../../Base/code/file.h\
          ../../Base/code/treebranch.h\


INCLUDEPATH  += ../../Base


DESTDIR = ../../Base/plugins
TARGET = $$qtLibraryTarget(jpeg_plugin)

copytool.commands = $(COPY_DIR) $$PWD/facedetect/ $$OUT_PWD/../../Base/tool
first.depends = $(first) copytool
export(first.depends)
export(copytool.commands)
QMAKE_EXTRA_TARGETS += first copytool

target.path =  /usr/local/bin/plugins

tools.files += tool/facedetect/facedetect\

tools.path =  /usr/local/bin/tool

INSTALLS += target tools

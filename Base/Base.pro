#-------------------------------------------------
#
# Project created by QtCreator 2015-03-01T14:46:04
#
#-------------------------------------------------

QT       += core gui xml


QMAKE_CXXFLAGS += -std=c++11

TARGET = SmartSorter
TEMPLATE = app


SOURCES  += main.cpp\
            mainwindow.cpp\
            code/control.cpp\
            code/lookup.cc\
            code/dectree.cc\
            code/analysis.cc\
            code/file.cc\
            code/node.cc\
            code/operation.cc \
            code/basic_classif.cpp\
            code/rule.cpp \
            code/treebranch.cpp \
            code/command.cpp \
            code/commandmanager.cpp \
            code/attribute.cpp \
            newrule.cpp \
            newdistribution.cpp \
            loadrule.cpp\

HEADERS  += mainwindow.h\
            code/control.h\
            code/lookup.h\
            code/dectree.h\
            code/analysis.h\
            code/file.h\
            code/node.h\
            code/operation.h \
            code/basic_classif.h\
            code/attribute.h \
            code/rule.h \
            code/treebranch.h \
            code/command.h \
            code/commandmanager.h \
            newrule.h \
            newdistribution.h \
            plugin_interface.h \
            loadrule.h\

FORMS    += mainwindow.ui \
            dialog.ui \
            newdistribution.ui \
            loadrule.ui\

RESOURCES = app_resources.qrc

copydata.commands = $(COPY_DIR) $$PWD/tool $$OUT_PWD
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata


tools.files += tool/fdupe.pl\
tools.files += tool/facedetect/facedetect\

tools.path =  /usr/local/bin/tool



target.path =  /usr/local/bin
INSTALLS += target tools tools2





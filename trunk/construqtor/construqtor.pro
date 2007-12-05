TEMPLATE = app

SOURCES += main.cpp \
mainwindow.cpp \
cqworld.cpp \
cqsimulation.cpp \
cqphysicalbody.cpp \
cqphysicalbox.cpp
FORMS += mainwindow.ui

HEADERS += mainwindow.h \
cqworld.h \
cqsimulation.h \
cqphysicalbody.h \
cqphysicalbox.h
CONFIG += debug \
qt \
warn_on
QT += core \
gui
TARGET = ../bin/construqtor

INCLUDEPATH += ../box2d \
../box2d/Collision \
../box2d/Dynamics \
../box2d/Common
LIBS += ../box2d/libbox2d.a

TARGETDEPS += ../box2d/libbox2d.a

OBJECTS_DIR = .obj

UI_DIR = .ui

MOC_DIR = .moc


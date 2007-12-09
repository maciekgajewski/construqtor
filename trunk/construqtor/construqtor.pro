TEMPLATE = app

SOURCES += main.cpp \
mainwindow.cpp \
cqworld.cpp \
cqsimulation.cpp \
cqphysicalbody.cpp \
cqphysicalbox.cpp \
cqmaterial.cpp \
cqjoint.cpp \
cqrevolutejoint.cpp \
cqnail.cpp \
cqitem.cpp \
mainview.cpp \
ceeditoritem.cpp
FORMS += mainwindow.ui

HEADERS += mainwindow.h \
cqworld.h \
cqsimulation.h \
cqphysicalbody.h \
cqphysicalbox.h \
cqmaterial.h \
cqjoint.h \
cqrevolutejoint.h \
cqnail.h \
cqitemtypes.h \
cqitem.h \
mainview.h \
ceeditoritem.h
CONFIG += debug \
qt \
warn_on \
rtti
QT += core \
gui
TARGET = ../bin/construqtor

OBJECTS_DIR = .obj

UI_DIR = .ui

MOC_DIR = .moc

INCLUDEPATH += ../box2d \
../box2d/Dynamics/Joints \
../box2d/Collision \
../box2d/Dynamics \
../box2d/Common
LIBS += ../box2d/libbox2d.a

TARGETDEPS += ../box2d/libbox2d.a


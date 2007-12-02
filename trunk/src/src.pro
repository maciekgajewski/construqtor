SOURCES += main.cpp \
drawwindow.cpp \
drawwidget.cpp \
world.cpp \
physicalobject.cpp \
polygontriangulator.cpp \
scenecontroller.cpp
TEMPLATE = app
CONFIG += warn_on \
	  thread \
          qt \
	  opengl \
	  x11 \
	  debug
FORMS += drawwindow.ui

HEADERS += drawwindow.h \
drawwidget.h \
world.h \
physicalobject.h \
polygontriangulator.h \
scenecontroller.h
QT += opengl

DISTFILES += ../README

TARGET = ../bin/qrayon

INCLUDEPATH += ../box2d \
../gpc \
../box2d/Dynamics/ \
../box2d/Common/
LIBS += -L../box2d \
-lbox2d \
../gpc/libgpc.a
TARGETDEPS += ../box2d/libbox2d.so \
../gpc/libgpc.a
OBJECTS_DIR = .obj

UI_DIR = .ui

MOC_DIR = .moc


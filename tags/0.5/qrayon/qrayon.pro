SOURCES += main.cpp \
drawwindow.cpp \
drawwidget.cpp \
world.cpp \
physicalobject.cpp \
polygontriangulator.cpp \
scenecontroller.cpp \
material.cpp
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
scenecontroller.h \
material.h
QT += opengl

DISTFILES += ../README

TARGET = ../bin/qrayon

OBJECTS_DIR = .obj

UI_DIR = .ui

MOC_DIR = .moc

INCLUDEPATH += ../box2d \
../gpc \
../box2d/Dynamics/ \
../box2d/Common/
LIBS += ../lib/libbox2d.a \
../lib/libgpc.a \
-L../box2d
TARGETDEPS += ../lib/libbox2d.a \
../lib/libgpc.a
RESOURCES += ../graphics/graphics.qrc


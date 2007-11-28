SOURCES += main.cpp \
drawwindow.cpp \
drawwidget.cpp \
world.cpp \
physicalobject.cpp
TEMPLATE = app
CONFIG += warn_on \
	  thread \
          qt \
	  opengl \
	  x11 \
	  debug
TARGET = ../bin/physics

FORMS += drawwindow.ui

HEADERS += drawwindow.h \
drawwidget.h \
world.h \
physicalobject.h
QT += opengl

INCLUDEPATH += ../box2d \
../box2d/Dynamics/ \
../box2d/Common/
LIBS += -L../bin \
-lbox2d \
-L/home/maciek/projekty/physics/bin
TARGETDEPS += ../bin/libbox2d.so


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
ceeditoritem.cpp \
cqgirder.cpp \
cqphysicaldisk.cpp \
cqwheel.cpp \
cqwheelwithengine.cpp \
cqcompounditem.cpp \
cqmotorcontroller.cpp \
cqrevolutevelocitycontroler.cpp \
controllerwidget.cpp \
cqbolt.cpp \
 cqpolygonalbody.cpp \
 cqpolygontriangulator.cpp \
 cqstone.cpp \
 cqgroundbody.cpp \
 cqfragilerevolutejoint.cpp \
 cqdocument.cpp \
 cqelement.cpp \
 cqitemfactory.cpp \
 cqgroupitem.cpp
FORMS += mainwindow.ui \
ControllerWidget.ui
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
ceeditoritem.h \
cqgirder.h \
cqphysicaldisk.h \
cqwheel.h \
cqwheelwithengine.h \
cqcompounditem.h \
cqmotorcontroller.h \
cqrevolutevelocitycontroler.h \
controllerwidget.h \
cqbolt.h \
 cqpolygonalbody.h \
 cqpolygontriangulator.h \
 cqstone.h \
 cqgroundbody.h \
 cqfragilerevolutejoint.h \
 gexception.h \
 cqdocument.h \
 cqelement.h \
 cqitemfactory.h \
 cqgroupitem.h
CONFIG += debug \
qt \
warn_on \
rtti
QT += core \
gui \
xml
TARGET = ../bin/construqtor

OBJECTS_DIR = .obj

UI_DIR = .ui

MOC_DIR = .moc

DISTFILES += FETUREPLAN

CONFIG -= release

INCLUDEPATH += ../box2d \
../gpc \
../box2d/Dynamics/Joints \
../box2d/Collision \
../box2d/Dynamics \
../box2d/Common
LIBS += ../box2d/libbox2d.a \
../gpc/libgpc.a
TARGETDEPS += ../box2d/libbox2d.a \
../gpc/libgpc.a

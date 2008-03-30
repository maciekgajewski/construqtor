SOURCES += Collision/b2BroadPhase.cpp \
Collision/b2CollideCircle.cpp \
Collision/b2CollidePoly.cpp \
Collision/b2Distance.cpp \
Collision/b2PairManager.cpp \
Collision/b2Shape.cpp \
Common/b2BlockAllocator.cpp \
Common/b2Settings.cpp \
Common/b2StackAllocator.cpp \
Dynamics/b2Body.cpp \
Dynamics/b2ContactManager.cpp \
Dynamics/b2Island.cpp \
Dynamics/b2WorldCallbacks.cpp \
Dynamics/b2World.cpp \
Dynamics/Contacts/b2CircleContact.cpp \
Dynamics/Contacts/b2Conservative.cpp \
Dynamics/Contacts/b2Contact.cpp \
Dynamics/Contacts/b2ContactSolver.cpp \
Dynamics/Contacts/b2PolyAndCircleContact.cpp \
Dynamics/Contacts/b2PolyContact.cpp \
Dynamics/Joints/b2DistanceJoint.cpp \
Dynamics/Joints/b2GearJoint.cpp \
Dynamics/Joints/b2Joint.cpp \
Dynamics/Joints/b2MouseJoint.cpp \
Dynamics/Joints/b2PrismaticJoint.cpp \
Dynamics/Joints/b2PulleyJoint.cpp \
Dynamics/Joints/b2RevoluteJoint.cpp
HEADERS += Collision/b2BroadPhase.h \
Collision/b2Collision.h \
Collision/b2PairManager.h \
Collision/b2Shape.h \
Common/b2BlockAllocator.h \
Common/b2Math.h \
Common/b2Settings.h \
Common/b2StackAllocator.h \
Dynamics/b2Body.h \
Dynamics/b2ContactManager.h \
Dynamics/b2Island.h \
Dynamics/b2WorldCallbacks.h \
Dynamics/b2World.h \
Dynamics/Contacts/b2CircleContact.h \
Dynamics/Contacts/b2Conservative.h \
Dynamics/Contacts/b2Contact.h \
Dynamics/Contacts/b2ContactSolver.h \
Dynamics/Contacts/b2NullContact.h \
Dynamics/Contacts/b2PolyAndCircleContact.h \
Dynamics/Contacts/b2PolyContact.h \
Dynamics/Joints/b2DistanceJoint.h \
Dynamics/Joints/b2GearJoint.h \
Dynamics/Joints/b2Joint.h \
Dynamics/Joints/b2MouseJoint.h \
Dynamics/Joints/b2PrismaticJoint.h \
Dynamics/Joints/b2PulleyJoint.h \
Dynamics/Joints/b2RevoluteJoint.h
TEMPLATE = lib

CONFIG += debug \
warn_on \
staticlib
DESTDIR = ../lib


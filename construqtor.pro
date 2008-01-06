SUBDIRS += \
box2d \
gpc\
construqtor\
qrayon
TEMPLATE = subdirs 
CONFIG += warn_on \
          qt \
          thread  \
          ordered
win32 {
	CONFIG += release
}

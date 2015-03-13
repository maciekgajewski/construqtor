# Build Instructions #

I will build windows and linux x86 (and maybe amd64) binaries on each version release.

How to build:

  1. Get Qt-4.3 or newer
  1. Get sources from Subversion repo
  1. Use qmake to generate makefile/VS project
  1. Build
  1. Enjoy

## Get Qt ##
Qt comes with each own development tools. It is available on any modern Linux distro. For windows, get Qt here: http://trolltech.com/developer/downloads/qt/windows

## Get source ##
See 'source' tab. You need subversion client.

## Configure ##
Use Qt's 'qmake' to generate project/makefile from constructor.pro.

## Build ##
Use build tool to which Qt was configured

On Linux box, I would do:
```
svn checkout http://construqtor.googlecode.com/svn/trunk/ construqtor-read-only
cd construqtor-read-only
qmake-qt4 constructor.pro
make
```
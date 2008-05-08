TEMPLATE = app
TARGET = 
QT     += opengl
CONFIG += debug
DEPENDPATH += .
INCLUDEPATH += . ../../include
DEFINES += COIN_DLL

LIBS += $(COINDIR)\lib\coin2d.lib ../../bin/SoQtViewer.lib

# Input
SOURCES += main.cpp

DESTDIR = ../../bin
OBJECTS_DIR = ./tmp/obj
MOC_DIR = ./tmp/moc
UI_HEADERS_DIR = ./tmp
UI_SOURCES_DIR = ./tmp

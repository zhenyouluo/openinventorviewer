TEMPLATE = app
TARGET = 
QT     += opengl

DEPENDPATH += .
INCLUDEPATH += . ../../include
DEFINES += COIN_DLL

CONFIG(debug, debug|release){
  TARGET = QtViewerd
  win32: LIBS += $(COINDIR)\lib\coin2d.lib 
  win32: LIBS += ../../bin/SoQtViewerd.lib
  unix:  LIBS += -lCoin -L../../bin -lSoQtViewerd
}

CONFIG(release, debug|release){
  TARGET = QtViewer
  win32: LIBS += $(COINDIR)\lib\coin2.lib 
  win32: LIBS += ../../bin/SoQtViewer.lib
  unix:  LIBS += -lCoin -L../../bin -lSoQtViewer
}

# Input
SOURCES += main.cpp

DESTDIR = ../../bin
OBJECTS_DIR = ./tmp/obj
MOC_DIR = ./tmp/moc
UI_HEADERS_DIR = ./tmp
UI_SOURCES_DIR = ./tmp

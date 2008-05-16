TEMPLATE = app
TARGET = 
QT     += opengl

DEPENDPATH += .
INCLUDEPATH += . ../../include
DEFINES += COIN_DLL

CONFIG(debug, debug|release){
  TARGET = QtViewerd
  LIBS += $(COINDIR)\lib\coin2d.lib 
  LIBS += ../../bin/SoQtViewerd.lib
}

CONFIG(release, debug|release){
  TARGET = QtViewer
  LIBS += $(COINDIR)\lib\coin2.lib 
  LIBS += ../../bin/SoQtViewer.lib
}

# Input
SOURCES += main.cpp

DESTDIR = ../../bin
OBJECTS_DIR = ./tmp/obj
MOC_DIR = ./tmp/moc
UI_HEADERS_DIR = ./tmp
UI_SOURCES_DIR = ./tmp

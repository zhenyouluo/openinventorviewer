TEMPLATE = lib
TARGET = SoQtViewer
win32: CONFIG += dll
QT     += opengl
DEFINES += COIN_DLL SOVIEWER_DLL

CONFIG(debug, debug|release){
  TARGET = $$join(TARGET,,,d)

  win32: LIBS += $(COINDIR)\lib\coin3d.lib
  unix:  LIBS += -lCoin
} else {
  
  win32: LIBS += $(COINDIR)\lib\coin3.lib
  unix:  LIBS += -lCoin
}

DEPENDPATH += . \
              include \
              src \
              include\Qt \
              src\Qt

INCLUDEPATH += . \
	      include \
		$(COINDIR)\include

# Input
HEADERS += include/SoViewer/SoRenderArea.h \
           include/SoViewer/Qt/SoQtRenderArea.h \
           include/SoViewer/Qt/SoQtViewer.h
SOURCES += src/SoRenderArea.cpp \
           src/Qt/SoQtRenderArea.cpp \
           src/Qt/SoQtViewer.cpp

DESTDIR = ./bin
OBJECTS_DIR = ./tmp/obj
MOC_DIR = ./tmp/moc
UI_HEADERS_DIR = ./tmp
UI_SOURCES_DIR = ./tmp

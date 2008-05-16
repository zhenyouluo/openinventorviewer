TEMPLATE = lib
CONFIG += dll
QT     += opengl
DEFINES += COIN_DLL SOVIEWER_DLL

CONFIG(debug, debug|release){
  TARGET = SoQtViewerd

  LIBS += $(COINDIR)\lib\coin2d.lib
}

CONFIG(release, debug|release){
  TARGET = SoQtViewer

  LIBS += $(COINDIR)\lib\coin2.lib
}

DEPENDPATH += . \
              include \
              src \
              include\Qt \
              src\Qt

INCLUDEPATH += . \
	      include

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

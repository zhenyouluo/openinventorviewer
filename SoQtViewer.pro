TEMPLATE = lib
CONFIG += dll debug
QT     += opengl
DEFINES += COIN_DLL SOVIEWER_DLL

DEPENDPATH += . \
              include \
              src \
              include\Qt \
              src\Qt

INCLUDEPATH += . \
	      include

LIBS += $(COINDIR)\lib\coin2d.lib

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

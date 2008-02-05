TEMPLATE = app
DEPENDPATH += .
INCLUDEPATH += .
QT  += opengl
CONFIG += opengl

win32: LIBS += inventor.lib

unix:  LIBS += libinventor.so

# Input
HEADERS += SoRenderArea.h SoQtRenderArea.h SoQtViewer.h
SOURCES += main.cpp SoRenderArea.cpp SoQtRenderArea.cpp SoQtViewer.cpp


TEMPLATE = lib
TARGET = 

QT  += opengl
CONFIG += opengl

DEPENDPATH += . ../include/Qt src/Qt
INCLUDEPATH += ../include

# Input
HEADERS += ../include/SoRenderArea.h \
           ../include/Qt/SoQtRenderArea.h \
           ../include/Qt/SoQtViewer.h
SOURCES += SoRenderArea.cpp \
           Qt/SoQtRenderArea.cpp \
           Qt/SoQtViewer.cpp

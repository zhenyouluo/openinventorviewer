TEMPLATE = app
TARGET = 
QT     += opengl
QT     -= qt
CONFIG += debug
DEPENDPATH += .
INCLUDEPATH += . ../../include \
	      $(WXWIN)\include \
	      $(WXWIN)\include\msvc

DEFINES += COIN_DLL WXUSINGDLL WXDEBUG __WXDEBUG__ 

LIBS += $(COINDIR)\lib\coin2d.lib ../../bin/SoWxViewer.lib
LIBS += $(WXWIN)\lib\vc_dll\wxmsw28d_core.lib 
LIBS += $(WXWIN)\lib\vc_dll\wxbase28d.lib 
LIBS += $(WXWIN)\lib\vc_dll\wxtiffd.lib 
LIBS += $(WXWIN)\lib\vc_dll\wxjpegd.lib 
LIBS += $(WXWIN)\lib\vc_dll\wxpngd.lib 
LIBS += $(WXWIN)\lib\vc_dll\wxzlibd.lib 
LIBS += $(WXWIN)\lib\vc_dll\wxregexd.lib 
LIBS += $(WXWIN)\lib\vc_dll\wxexpatd.lib
LIBS += $(WXWIN)\lib\vc_dll\wxmsw28d_gl.lib

# Input
SOURCES += main.cpp

DESTDIR = ../../bin
OBJECTS_DIR = ./tmp/obj
MOC_DIR = ./tmp/moc
UI_HEADERS_DIR = ./tmp
UI_SOURCES_DIR = ./tmp

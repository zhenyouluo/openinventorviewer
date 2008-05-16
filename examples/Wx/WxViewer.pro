TEMPLATE = app
CONFIG += opengl
CONFIG -= qt

DEFINES += COIN_DLL WXUSINGDLL 
DEFINES -= UNICODE QT_LARGEFILE_SUPPORT

CONFIG(debug, debug|release){
  DEFINES += WXDEBUG

  TARGET = WxViewerd

  LIBS += $(COINDIR)\lib\coin2d.lib 
  LIBS += ../../bin/SoWxViewerd.lib

  LIBS += $(WXWIN)\lib\vc_dll\wxmsw28d_core.lib
  LIBS += $(WXWIN)\lib\vc_dll\wxbase28d.lib 
  LIBS += $(WXWIN)\lib\vc_dll\wxtiffd.lib 
  LIBS += $(WXWIN)\lib\vc_dll\wxjpegd.lib 
  LIBS += $(WXWIN)\lib\vc_dll\wxpngd.lib 
  LIBS += $(WXWIN)\lib\vc_dll\wxzlibd.lib 
  LIBS += $(WXWIN)\lib\vc_dll\wxregexd.lib 
  LIBS += $(WXWIN)\lib\vc_dll\wxexpatd.lib
  LIBS += $(WXWIN)\lib\vc_dll\wxmsw28d_gl.lib
}

CONFIG(release, debug|release){
  TARGET = WxViewer

  LIBS += $(COINDIR)\lib\coin2.lib 
  LIBS += ../../bin/SoWxViewer.lib

  LIBS += $(WXWIN)\lib\vc_dll\wxmsw28_core.lib
  LIBS += $(WXWIN)\lib\vc_dll\wxbase28.lib 
  LIBS += $(WXWIN)\lib\vc_dll\wxtiff.lib 
  LIBS += $(WXWIN)\lib\vc_dll\wxjpeg.lib 
  LIBS += $(WXWIN)\lib\vc_dll\wxpng.lib 
  LIBS += $(WXWIN)\lib\vc_dll\wxzlib.lib 
  LIBS += $(WXWIN)\lib\vc_dll\wxregex.lib 
  LIBS += $(WXWIN)\lib\vc_dll\wxexpat.lib
  LIBS += $(WXWIN)\lib\vc_dll\wxmsw28_gl.lib
}


DEPENDPATH += .
INCLUDEPATH += . ../../include \
	      $(WXWIN)\include \
	      $(WXWIN)\include\msvc

# Input
SOURCES += main.cpp

DESTDIR = ../../bin
OBJECTS_DIR = ./tmp/obj
MOC_DIR = ./tmp/moc
UI_HEADERS_DIR = ./tmp
UI_SOURCES_DIR = ./tmp

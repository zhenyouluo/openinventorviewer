TEMPLATE = app
TARGET = WxViewer
CONFIG += opengl
CONFIG -= qt

DEFINES += COIN_DLL WXUSINGDLL 
DEFINES -= UNICODE QT_LARGEFILE_SUPPORT

CONFIG(debug, debug|release){
  DEFINES += WXDEBUG

  TARGET = $$join(TARGET,,d)

  LIBS += $(COINDIR)\lib\coin3d.lib 
  LIBS += ../../bin/SoWxViewerd.lib
  LIBS += -L$(WXWIN)\lib\vc_dll\

  LIBS += wxmsw28d_core.lib
  LIBS += wxbase28d.lib 
  LIBS += wxtiffd.lib 
  LIBS += wxjpegd.lib 
  LIBS += wxpngd.lib 
  LIBS += wxzlibd.lib 
  LIBS += wxregexd.lib 
  LIBS += wxexpatd.lib
  LIBS += wxmsw28d_gl.lib
} else {
  LIBS += $(COINDIR)\lib\coin3.lib 
  LIBS += ../../bin/SoWxViewer.lib
  LIBS += -L$(WXWIN)\lib\vc_dll\


  LIBS += wxmsw28_core.lib
  LIBS += wxbase28.lib 
  LIBS += wxtiff.lib 
  LIBS += wxjpeg.lib 
  LIBS += wxpng.lib 
  LIBS += wxzlib.lib 
  LIBS += wxregex.lib 
  LIBS += wxexpat.lib
  LIBS += wxmsw28_gl.lib
}


DEPENDPATH += .
INCLUDEPATH += . ../../include \
		$(COINDIR)\include \
	      $(WXWIN)\include \
	      $(WXWIN)\include\msvc

# Input
SOURCES += main.cpp

DESTDIR = ../../bin
OBJECTS_DIR = ./tmp/obj
MOC_DIR = ./tmp/moc
UI_HEADERS_DIR = ./tmp
UI_SOURCES_DIR = ./tmp

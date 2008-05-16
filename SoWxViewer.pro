TEMPLATE = lib
CONFIG += dll opengl 
CONFIG -= qt

DEFINES += COIN_DLL SOVIEWER_DLL WXUSINGDLL

CONFIG(debug, debug|release){
  DEFINES += WXDEBUG
  TARGET = SoWxViewerd

  LIBS += $(COINDIR)\lib\coin2d.lib

  LIBS += $(WXWIN)\lib\vc_dll\wxmsw28d_core.lib 
  LIBS += $(WXWIN)\lib\vc_dll\wxbase28d.lib 
  LIBS += $(WXWIN)\lib\vc_dll\wxtiffd.lib 
  LIBS += $(WXWIN)\lib\vc_dll\wxjpegd.lib 
  LIBS += $(WXWIN)\lib\vc_dll\wxpngd.lib 
  LIBS += $(WXWIN)\lib\vc_dll\wxzlibd.lib 
  LIBS += $(WXWIN)\lib\vc_dll\wxregexd.lib 
  LIBS += $(WXWIN)\lib\vc_dll\wxexpatd.lib
}

CONFIG(release,debug|release){
  TARGET = SoWxViewer

  LIBS += $(COINDIR)\lib\coin2.lib

  LIBS += $(WXWIN)\lib\vc_dll\wxmsw28_core.lib 
  LIBS += $(WXWIN)\lib\vc_dll\wxbase28.lib 
  LIBS += $(WXWIN)\lib\vc_dll\wxtiff.lib 
  LIBS += $(WXWIN)\lib\vc_dll\wxjpeg.lib 
  LIBS += $(WXWIN)\lib\vc_dll\wxpng.lib 
  LIBS += $(WXWIN)\lib\vc_dll\wxzlib.lib 
  LIBS += $(WXWIN)\lib\vc_dll\wxregex.lib 
  LIBS += $(WXWIN)\lib\vc_dll\wxexpat.lib
}

DEPENDPATH += . \
              include \
              src \
              include\Wx \
              src\Wx

INCLUDEPATH += . \
	      include \
	      $(WXWIN)\include \
	      $(WXWIN)\include\msvc

# Input
HEADERS += include/SoViewer/SoRenderArea.h \
           include/SoViewer/Wx/SoWxRenderArea.h \
           include/SoViewer/Wx/SoWxViewer.h
SOURCES += src/SoRenderArea.cpp \
           src/Wx/SoWxRenderArea.cpp \
           src/Wx/SoWxViewer.cpp

DESTDIR = ./bin
OBJECTS_DIR = ./tmp/obj
MOC_DIR = ./tmp/moc
UI_HEADERS_DIR = ./tmp
UI_SOURCES_DIR = ./tmp

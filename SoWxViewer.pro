TEMPLATE = lib
TARGET = SoWxViewer

CONFIG += dll opengl 
CONFIG -= qt

DEFINES += COIN_DLL SOVIEWER_DLL WXUSINGDLL

CONFIG(debug, debug|release){
  DEFINES += WXDEBUG
  TARGET = $$join(TARGET,,,d) 

  LIBS += $(COINDIR)\lib\coin3d.lib
  LIBS += -L$(WXWIN)\lib\vc_dll\

  LIBS += wxmsw28d_core.lib 
  LIBS += wxbase28d.lib 
  LIBS += wxtiffd.lib 
  LIBS += wxjpegd.lib 
  LIBS += wxpngd.lib 
  LIBS += wxzlibd.lib 
  LIBS += wxregexd.lib 
  LIBS += wxexpatd.lib
} else {
  LIBS += $(COINDIR)\lib\coin3.lib
  LIBS += -L$(WXWIN)\lib\vc_dll\

  LIBS += wxmsw28_core.lib 
  LIBS += wxbase28.lib 
  LIBS += wxtiff.lib 
  LIBS += wxjpeg.lib 
  LIBS += wxpng.lib 
  LIBS += wxzlib.lib 
  LIBS += wxregex.lib 
  LIBS += wxexpat.lib
}

DEPENDPATH += . \
              include \
              src \
              include\Wx \
              src\Wx

INCLUDEPATH += . \
	      include \
            $(COINDIR)\include \
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

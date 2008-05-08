#if (QT_VERSION >= 0x40000 )
#   include <QApplication>
#else
#   include <qapplication.h>
#endif

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/manips/SoTrackballManip.h>

#include <SoViewer/Qt/SoQtViewer.h>

int main(int argc, char **argv)
{
  QApplication app( argc, argv );

  SoQtViewer * myViewer = new SoQtViewer;

#if QT_VERSION < 0x040000
  app.setMainWidget( myViewer );
#endif

  SoSeparator * root = new SoSeparator;
  root->ref();

  root->addChild( new SoTrackballManip );
  root->addChild( new SoCube );
  
  myViewer->setBackgroundColor(SbColor(0.68f,0.7f,0.8f));
  myViewer->setSceneGraph(root);
  myViewer->show();
  myViewer->viewAll();

  return app.exec();
}

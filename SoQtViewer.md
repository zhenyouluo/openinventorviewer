# Introduction #

Qt 3.x/4.x version of the viewer and the render area.

```
#include <QApplication>

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/manips/SoTrackballManip.h>

#include <SoViewer/Qt/SoQtViewer.h>

int main(int argc, char **argv)
{
  QApplication app( argc, argv );

  SoDB::init();
  SoNodeKit::init();
  SoInteraction::init();
  
  SoQtViewer * myViewer = new SoQtViewer;

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
```
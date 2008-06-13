/*
_______________________________________________________________________
__________________________ S O V I E W E R ____________________________
|
| THIS FILE IS PART OF THE SOVIEWER LIBRARY.
| USE, DISTRIBUTION AND REPRODUCTION OF THIS LIBRARY SOURCE IS     
| GOVERNED BY A BSD-STYLE SOURCE LICENSE.
| PLEASE READ THESE TERMS BEFORE DISTRIBUTING.       
_______________________________________________________________________
_______________________________________________________________________
*/


#ifndef _SOQTRENDERAREA_H_
#define _SOQTRENDERAREA_H_

#include <qglobal.h>

#if QT_VERSION >= 0x040000
#   include <QGLWidget>
#   include <QTimer>
#   include <QDateTime>
#else
#   include <qgl.h>
#   include <qtimer.h>
#   include <qdatetime.h>
#endif

#include <SoViewer/SoRenderArea.h>

class SOVIEWER_API SoQtRenderArea : public QGLWidget, public SoRenderArea
{
    Q_OBJECT

public:
#if QT_VERSION >= 0x040000 
    SoQtRenderArea( QWidget * parent = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0 );
    SoQtRenderArea( QGLContext * context, QWidget * parent = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0 );
    SoQtRenderArea( const QGLFormat & format, QWidget * parent = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0 );
#else    
    SoQtRenderArea( QWidget * parent = 0, const char * name = 0, const QGLWidget * shareWidget = 0, WFlags f = 0 ); 
    SoQtRenderArea( QGLContext * context, QWidget * parent, const char * name = 0, const QGLWidget * shareWidget = 0, WFlags f = 0 );
    SoQtRenderArea( const QGLFormat & format, QWidget * parent = 0, const char * name = 0, const QGLWidget * shareWidget = 0, WFlags f = 0 );
#endif
    virtual ~SoQtRenderArea();

    // This method should be overridden due to the same method name existence in QWidget.
    virtual void setBackgroundColor(const SbColor & a_color){ SoRenderArea::setBackgroundColor(a_color); }
protected:
    virtual void soRenderCallback();

    virtual void initializeGL();
    virtual void resizeGL( int width, int height );
    virtual void paintGL();
    virtual void timerEvent( QTimerEvent * e );
    virtual void keyPressEvent( QKeyEvent * e );
    virtual void keyReleaseEvent( QKeyEvent * e );
    virtual void mouseMoveEvent( QMouseEvent * e );
    virtual void mousePressEvent( QMouseEvent * e );
    virtual void mouseReleaseEvent( QMouseEvent * e );
    virtual void wheelEvent( QWheelEvent * e );
private:
    void commonInit();

    static unsigned int  m_cache_context;

    QTime                m_time;

    SoKeyboardEvent::Key translateKey( QKeyEvent * e );

    int m_timerSensorId;
    int m_delaySensorId;
};

#endif // _SOQTRENDERAREA_H_

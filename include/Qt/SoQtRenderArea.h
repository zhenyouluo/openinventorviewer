// ***************************************************************************
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
//           (C) Copyright 2000-2002 Morgan Leborgne
//
//    Contact: http://morgan.leborgne.free.fr
//             mailto: morgan.leborgne@free.fr
// ***************************************************************************

#ifndef _SOQTRENDERAREA_H_
#define _SOQTRENDERAREA_H_

#if (QT_VERSION >= 0x40000 )
#   include <QGLWidget>
#   include <QTimer>
#   include <QDateTime>
#else
#   include <qgl.h>
#   include <qtimer.h>
#   include <qdatetime.h>
#endif

#include <SoRenderArea.h>

class SoQtRenderArea : public QGLWidget, public SoRenderArea
{
	Q_OBJECT

public:
	SoQtRenderArea(QWidget *parent=0);
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
	static unsigned int  m_cache_context;

	QTime                m_time;

	SoKeyboardEvent::Key translateKey( QKeyEvent * e );
};

#endif // _SOQTRENDERAREA_H_

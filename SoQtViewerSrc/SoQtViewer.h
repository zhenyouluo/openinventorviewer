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

#ifndef _SOQTVIEWER_H_
#define _SOQTVIEWER_H_

#include "SoQtRenderArea.h"

#include <Inventor/actions/SoGetBoundingBoxAction.h>

class SoCamera;
class SoNode;
class SoSeparator;
class SoDirectionalLight;

class SoQtViewer : public SoQtRenderArea
{
    Q_OBJECT
public:
    SoQtViewer(QWidget *parent=0);

    virtual ~SoQtViewer();

    /*! Sets the edited camera. Setting the camera is only needed if the first camera found
        in the scene when setting the scene graph isn't the one the user really wants to edit. */
    virtual void setCamera(SoCamera *camera);

    //! Gets the edited camera. 
    SoCamera * getCamera();

    //! Sets the scene graph to be rendered in this component's window. 
    virtual void setSceneGraph(SoNode * a_new_scene);
    
    /*! Sets the camera type that will be created by the viewer if no cameras are found in 
        the scene graph (see SoPerspectiveCamera and SoOrthographicCamera). 
        By default an SoPerspectiveCamera will be created if no cameras are found. */
    virtual void setCameraType(SoType type);
    
    //! Gets the scene graph to be rendered in this component's window.
    virtual SoNode * getSceneGraph();
    
    //! Changes the camera position to view the entire scene (the camera zoom or orientation isn't changed).
    virtual void viewAll();

	/*! Set view mode.
		If the view mode is on, user events will be caught and used to influence the camera position / orientation. 
		If view mode is off, all events in the viewer canvas (like for instance keypresses or mouseclicks and -movements) will be passed along to the scene graph.
		Default is to have the view mode active. */
	virtual void setViewing(bool state);

	/*! Return state of view mode.
		true means that the mode of the viewer is set such that user interaction with the mouse is used to modify the position and orientation of the camera. */
	bool isViewing() const{ return m_viewing; }

    SbVec3f projectPoint(const SbVec2f & pt) const;
    void rotateCamera(const SbRotation &rot);
    void adjustCameraClippingPlanes();
protected:
    virtual void soPaintEvent();
	virtual void soKeyPressEvent( SoKeyboardEvent * e);
    virtual void soMousePressEvent( SoMouseButtonEvent * e );
    virtual void soMouseMoveEvent( SoLocation2Event * e );
	virtual void soMouseReleaseEvent( SoMouseButtonEvent * e );
    virtual void soWheelEvent( SoWheelEvent * e );
private:
    SoType               m_cameraType;
    SoCamera           * m_p_camera;
    SoSeparator        * m_p_rootGraph;
    SoNode             * m_p_sceneGraph;
    SoDirectionalLight * m_p_directionalLight;

    SbVec2f              m_startDrag;
	bool				 m_viewing;

	SoMouseButtonEvent::Button m_mouseButton;
};

#endif
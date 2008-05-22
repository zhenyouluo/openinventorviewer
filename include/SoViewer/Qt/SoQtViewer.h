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

#ifndef _SOQTVIEWER_H_
#define _SOQTVIEWER_H_

#include <SoViewer/Qt/SoQtRenderArea.h>

#include <Inventor/actions/SoGetBoundingBoxAction.h>

class SoCamera;
class SoNode;
class SoSeparator;
class SoDirectionalLight;

class SOVIEWER_API SoQtViewer : public SoQtRenderArea
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
    bool                 m_viewing;

    SoMouseButtonEvent::Button m_mouseButton;
};

#endif

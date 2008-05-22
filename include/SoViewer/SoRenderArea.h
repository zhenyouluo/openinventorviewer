/*
_______________________________________________________________________
__________________________ G E O M E T R Y ____________________________
|
| THIS FILE IS PART OF THE SOVIEWER LIBRARY.
| USE, DISTRIBUTION AND REPRODUCTION OF THIS LIBRARY SOURCE IS     
| GOVERNED BY A BSD-STYLE SOURCE LICENSE.
| PLEASE READ THESE TERMS BEFORE DISTRIBUTING.       
_______________________________________________________________________
_______________________________________________________________________
*/
#ifndef _SORENDERAREA_H_
#define _SORENDERAREA_H_

#ifdef _WIN32
#  ifdef SOVIEWER_DLL
#     define SOVIEWER_API __declspec(dllexport)
#  else
#     define SOVIEWER_API __declspec(dllimport)
#endif
#else
#   define SOVIEWER_API 
#endif

class SoTempPath;
#include <Inventor/nodes/SoNode.h>
#include <Inventor/SoSceneManager.h> 
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/events/SoLocation2Event.h> 

//The SoWheelEvent class contains parameters that describe a mouse wheel event.
class SOVIEWER_API SoWheelEvent : public SoEvent
{
    SO_EVENT_HEADER();
public:
    SoWheelEvent(){}

    void setDelta(int delta){ m_delta = delta; }
    int getDelta() const{ return m_delta; }
private:
    int m_delta;
};

/** The SoRenderArea interface adds scenegraph handling and event management.
Application programmers will not use this class directly, but rather through the implementation of either its direct descendant; 
SoQtRenderArea, SoWxRenderArea, or through one of the "rapid application development"-style viewer subclasses.
*/
class SOVIEWER_API SoRenderArea
{
public:
    SoRenderArea();
    virtual ~SoRenderArea();

    //! Sets the quality level for rendering transparent objects. See SoGLRenderAction for possible transparency types.
    void setTransparencyType(SoGLRenderAction::TransparencyType a_type);

    //! Sets the scene graph to be rendered in this component's window. 
    virtual void setSceneGraph(SoNode * a_new_scene);

    //! Gets the scene graph to be rendered in this component's window.
    virtual SoNode * getSceneGraph(){ return m_p_scene_manager->getSceneGraph();}

    //! Sets the background color for this window. Default is black (0,0,0). 
    virtual void setBackgroundColor(const SbColor & a_color){ m_p_scene_manager->setBackgroundColor(a_color); }

    //! Gets the background color for this window. 
    const SbColor & getBackgroundColor() const { return m_p_scene_manager->getBackgroundColor(); }

    //! Gets the normal scene manager.
    SoSceneManager * getSceneManager(){ return m_p_scene_manager; }

    //! Sets viewport region to use for rendering. 
    void setViewportRegion(const SbViewportRegion & a_region) { m_p_scene_manager->getGLRenderAction()->setViewportRegion(a_region); }

    //! Gets current viewport region to use for rendering. 
    const SbViewportRegion & getViewportRegion() const { return m_p_scene_manager->getGLRenderAction()->getViewportRegion(); }

protected:
    virtual void soKeyPressEvent( SoKeyboardEvent * e);
    virtual void soKeyReleaseEvent( SoKeyboardEvent * e );
    virtual void soMouseMoveEvent( SoLocation2Event * e );
    virtual void soMousePressEvent( SoMouseButtonEvent * e );
    virtual void soMouseReleaseEvent( SoMouseButtonEvent * e );
    virtual void soWheelEvent( SoWheelEvent * e );
    virtual void soRenderCallback() = 0; 
    virtual void soResizeEvent(int width, int height);
    virtual void soPaintEvent();

    SoSceneManager      * m_p_scene_manager;
    SoKeyboardEvent     * m_keyboard_event;
    SoMouseButtonEvent  * m_mouse_button_event;
    SoLocation2Event    * m_location_event;
    SoWheelEvent        * m_wheel_event;

    static void renderCallback(void *userData, SoSceneManager *mgr);
};

#endif // _SORENDERAREA_H_

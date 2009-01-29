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
#include <SoViewer/SoRenderArea.h>

#include <Inventor/SoDB.h>
#include <Inventor/SbColor.h>
#include <Inventor/nodekits/SoNodeKit.h> 
#include <Inventor/SoInteraction.h>

#ifdef __COIN__
#	include <Inventor/elements/SoGLCacheContextElement.h>
#endif

SO_EVENT_SOURCE(SoWheelEvent);

void SoRenderArea::renderCallback(void *userData, SoSceneManager *)
{
    ((SoRenderArea*)userData)->soRenderCallback();
}

SoRenderArea::SoRenderArea()
{
    SoDB::init();
    SoNodeKit::init();
    SoInteraction::init();

    m_keyboard_event = new SoKeyboardEvent;
    m_mouse_button_event = new SoMouseButtonEvent;
    m_location_event = new SoLocation2Event;
    m_wheel_event = new SoWheelEvent;

    m_p_scene_manager = new SoSceneManager;
	m_p_scene_manager->getGLRenderAction()->setCacheContext( this->getCacheContextId() );
    m_p_scene_manager->setBackgroundColor(SbColor(0.0f, 0.0f, 0.0f)); //BLACK COLOR
    m_p_scene_manager->setRenderCallback(renderCallback, this);
}

SoRenderArea::~SoRenderArea()
{
    delete m_p_scene_manager;
    delete m_keyboard_event;
    delete m_mouse_button_event;
    delete m_location_event;
    delete m_wheel_event;
}

void SoRenderArea::setTransparencyType(SoGLRenderAction::TransparencyType a_type)
{
    m_p_scene_manager->getGLRenderAction()->setTransparencyType(a_type);

    m_p_scene_manager->scheduleRedraw();
}

void SoRenderArea::setSceneGraph(SoNode * a_new_scene)
{
    m_p_scene_manager->deactivate();

    m_p_scene_manager->setSceneGraph( a_new_scene );

    m_p_scene_manager->activate();

    m_p_scene_manager->scheduleRedraw();
}

void SoRenderArea::setGLRenderAction(SoGLRenderAction *ra)
{ 
    m_p_scene_manager->setGLRenderAction(ra); 
}

SoGLRenderAction * SoRenderArea::getGLRenderAction() const
{ 
    return m_p_scene_manager->getGLRenderAction(); 
}

unsigned int SoRenderArea::getCacheContextId() const
{
#ifdef __COIN__
	return SoGLCacheContextElement::getUniqueCacheContext();
#else
	static unsigned int s_cache_context = 19720408;
	return s_cache_context++;
#endif
}

void SoRenderArea::soKeyPressEvent( SoKeyboardEvent * e)
{
    m_p_scene_manager->processEvent( e );
}

void SoRenderArea::soKeyReleaseEvent( SoKeyboardEvent * e )
{
    m_p_scene_manager->processEvent( e );
}

void SoRenderArea::soMouseMoveEvent( SoLocation2Event * e )
{
    m_p_scene_manager->processEvent( e );
}

void SoRenderArea::soMousePressEvent( SoMouseButtonEvent * e )
{
    m_p_scene_manager->processEvent( e );
}

void SoRenderArea::soMouseReleaseEvent( SoMouseButtonEvent * e )
{
    m_p_scene_manager->processEvent( e );
}

void SoRenderArea::soWheelEvent( SoWheelEvent * e )
{
    m_p_scene_manager->processEvent( e );
}

void SoRenderArea::soResizeEvent(int width, int height)
{
    SbVec2s size((short)width, (short) height);

    m_p_scene_manager->setWindowSize(size);
    m_p_scene_manager->setSize(size);
    m_p_scene_manager->setViewportRegion(SbViewportRegion(size));
    m_p_scene_manager->scheduleRedraw();
}

void SoRenderArea::soPaintEvent()
{
    m_p_scene_manager->render();
}

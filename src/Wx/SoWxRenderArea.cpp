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
#include "SoWxRenderArea.h"

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#	include "wx/wx.h"
#endif

#include <Inventor/SoDB.h>
#include <Inventor/nodekits/SoNodeKit.h> 
#include <Inventor/SoInteraction.h>

#define TIMER_ID 10100

BEGIN_EVENT_TABLE(SoWxRenderArea, wxGLCanvas)
EVT_SIZE(SoWxRenderArea::OnSize)
EVT_PAINT(SoWxRenderArea::OnPaint)
EVT_ERASE_BACKGROUND(SoWxRenderArea::OnEraseBackground)
EVT_KEY_DOWN(SoWxRenderArea::OnKeyDown)
EVT_KEY_UP(SoWxRenderArea::OnKeyUp)
EVT_MOTION(SoWxRenderArea::OnMouseMove)
EVT_MOUSEWHEEL(SoWxRenderArea::OnMouseWheel)
EVT_LEFT_DOWN(SoWxRenderArea::OnMousePress)
EVT_MIDDLE_DOWN(SoWxRenderArea::OnMousePress)
EVT_RIGHT_DOWN(SoWxRenderArea::OnMousePress)
EVT_LEFT_UP(SoWxRenderArea::OnMouseRelease)
EVT_MIDDLE_UP(SoWxRenderArea::OnMouseRelease)
EVT_RIGHT_UP(SoWxRenderArea::OnMouseRelease)
EVT_TIMER(TIMER_ID, SoWxRenderArea::OnTimerEvent)
END_EVENT_TABLE()


SoWxRenderArea::SoWxRenderArea(wxWindow *parent, wxWindowID id,
                               const wxPoint& pos, const wxSize& size, long style, const wxString& name)
                               : wxGLCanvas(parent, id, pos, size, style|wxFULL_REPAINT_ON_RESIZE, name)
{
    //m_p_scene_manager->getGLRenderAction()->setCacheContext(m_cache_context++);

    m_time.SetOwner(this, TIMER_ID);
    m_time.Start( 1 );
}

SoWxRenderArea::~SoWxRenderArea()
{

}

void SoWxRenderArea::soRenderCallback()
{
    Refresh();
}

void SoWxRenderArea::OnTimerEvent(wxTimerEvent& event)
{
    SoDB::getSensorManager()->processTimerQueue();
    SoDB::getSensorManager()->processDelayQueue(TRUE);
}

void SoWxRenderArea::OnKeyDown(wxKeyEvent& e)
{
    m_keyboard_event->setTime(SbTime(e.GetTimestamp() / 1000.0));
    m_keyboard_event->setKey( translateKey(e) );
    m_keyboard_event->setState(SoButtonEvent::DOWN);

    m_keyboard_event->setShiftDown(e.ShiftDown());
    m_keyboard_event->setCtrlDown(e.ControlDown());
    m_keyboard_event->setAltDown(e.AltDown());

    soKeyPressEvent( m_keyboard_event );
}

void SoWxRenderArea::OnKeyUp(wxKeyEvent& e)
{
    m_keyboard_event->setTime(SbTime(e.GetTimestamp() / 1000.0));
    m_keyboard_event->setKey( translateKey(e) );
    m_keyboard_event->setState(SoButtonEvent::UP);

    m_keyboard_event->setShiftDown(e.ShiftDown());
    m_keyboard_event->setCtrlDown(e.ControlDown());
    m_keyboard_event->setAltDown(e.AltDown());

    soKeyReleaseEvent( m_keyboard_event );
}

void SoWxRenderArea::OnMousePress(wxMouseEvent& e)
{
    switch( e.GetButton() )
    {
    case 1: m_mouse_button_event->setButton(SoMouseButtonEvent::BUTTON1); break;
    case 2:	m_mouse_button_event->setButton(SoMouseButtonEvent::BUTTON2); break;
    case 3:	m_mouse_button_event->setButton(SoMouseButtonEvent::BUTTON3); break;
    default:m_mouse_button_event->setButton(SoMouseButtonEvent::ANY);     break;
    } 
    m_mouse_button_event->setTime(SbTime(e.GetTimestamp() / 1000.0));
    m_mouse_button_event->setState(SoButtonEvent::DOWN);

    int height = GetClientSize().GetHeight();

    m_mouse_button_event->setPosition(SbVec2s(e.GetX(), height - 1 - e.GetY()));

    m_mouse_button_event->setShiftDown(e.ShiftDown());
    m_mouse_button_event->setCtrlDown(e.ControlDown());
    m_mouse_button_event->setAltDown(e.AltDown());

    soMousePressEvent( m_mouse_button_event );
}

void SoWxRenderArea::OnMouseMove(wxMouseEvent& e)
{
    m_location_event->setTime(SbTime(e.GetTimestamp() / 1000.0));

    int height = GetClientSize().GetHeight();

    m_location_event->setPosition(SbVec2s(e.GetX(), height - 1 - e.GetY()));
    m_location_event->setShiftDown(e.ShiftDown());
    m_location_event->setCtrlDown(e.ControlDown());
    m_location_event->setAltDown(e.AltDown());

    soMouseMoveEvent( m_location_event );
}

void SoWxRenderArea::OnMouseRelease(wxMouseEvent& e)
{
    switch( e.GetButton() )
    {
    case 1: m_mouse_button_event->setButton(SoMouseButtonEvent::BUTTON1); break;
    case 2:	m_mouse_button_event->setButton(SoMouseButtonEvent::BUTTON2); break;
    case 3:	m_mouse_button_event->setButton(SoMouseButtonEvent::BUTTON3); break;
    default:m_mouse_button_event->setButton(SoMouseButtonEvent::ANY);     break;
    } 

    m_mouse_button_event->setTime(SbTime(e.GetTimestamp() / 1000.0));
    m_mouse_button_event->setState(SoButtonEvent::UP);

    int height = GetClientSize().GetHeight();

    m_mouse_button_event->setPosition(SbVec2s(e.GetX(), height - 1 - e.GetY()));
    m_mouse_button_event->setShiftDown(e.ShiftDown());
    m_mouse_button_event->setCtrlDown(e.ControlDown());
    m_mouse_button_event->setAltDown(e.AltDown());

    soMouseReleaseEvent( m_mouse_button_event );
}

void SoWxRenderArea::OnMouseWheel(wxMouseEvent& e)
{
    int height = GetClientSize().GetHeight();

    m_wheel_event->setTime(SbTime(e.GetTimestamp() / 1000.0));
    m_mouse_button_event->setPosition(SbVec2s(e.GetX(), height - 1 - e.GetY()));

    m_wheel_event->setShiftDown(e.ShiftDown());
    m_wheel_event->setCtrlDown(e.ControlDown());
    m_wheel_event->setAltDown(e.AltDown());
    m_wheel_event->setDelta(e.GetWheelRotation());
    soWheelEvent( m_wheel_event );
}

void SoWxRenderArea::OnPaint(wxPaintEvent& /*event*/)
{
    wxPaintDC dc(this);

    SetCurrent();
    initializeGL();

    soPaintEvent();

    SwapBuffers();
}

void SoWxRenderArea::initializeGL()
{ 
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); 

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}

void SoWxRenderArea::OnSize(wxSizeEvent& event)
{
    // this is also necessary to update the context on some platforms
    wxGLCanvas::OnSize(event);

    SetCurrent();

    wxSize wxsize = wxGLCanvas::GetClientSize();

    soResizeEvent(wxsize.GetWidth(), wxsize.GetHeight());
}

void SoWxRenderArea::OnEraseBackground(wxEraseEvent& /*event*/)
{
    // Do nothing, to avoid flashing on MSW
}

SoKeyboardEvent::Key SoWxRenderArea::translateKey( wxKeyEvent& e )
{
    SoKeyboardEvent::Key event = SoKeyboardEvent::ANY;

    switch(e.GetKeyCode())
    {
    case WXK_BACK:	event = SoKeyboardEvent::BACKSPACE; break;
    case WXK_TAB:	event = SoKeyboardEvent::TAB; break;
    case WXK_RETURN:	event = SoKeyboardEvent::RETURN; break;
    case WXK_ESCAPE:	event = SoKeyboardEvent::ESCAPE; break;
    case WXK_SPACE:	event = SoKeyboardEvent::SPACE; break;
    case WXK_DELETE:	event = SoKeyboardEvent::KEY_DELETE; break;
    case WXK_START: break;
    case WXK_LBUTTON: break;
    case WXK_RBUTTON: break;
    case WXK_CANCEL: break;
    case WXK_MBUTTON: break;
    case WXK_CLEAR: break;
    case WXK_SHIFT:     event = SoKeyboardEvent::LEFT_SHIFT; break;
    case WXK_CONTROL:   event = SoKeyboardEvent::LEFT_CONTROL; break;
    case WXK_MENU: break;
    case WXK_PAUSE:     event = SoKeyboardEvent::PAUSE; break;
    case WXK_CAPITAL: break;
    case WXK_END: break;
    case WXK_HOME: break;
    case WXK_LEFT: break;
    case WXK_UP: break;
    case WXK_RIGHT: break;
    case WXK_DOWN: break;
    case WXK_SELECT: break;
    case WXK_PRINT: break;
    case WXK_EXECUTE: break;
    case WXK_SNAPSHOT: break;
    case WXK_INSERT: break;
    case WXK_HELP: break;
    case WXK_NUMPAD0:   event = SoKeyboardEvent::NUMBER_0; break;
    case WXK_NUMPAD1:   event = SoKeyboardEvent::NUMBER_1; break;
    case WXK_NUMPAD2:   event = SoKeyboardEvent::NUMBER_2; break;
    case WXK_NUMPAD3:   event = SoKeyboardEvent::NUMBER_3; break;
    case WXK_NUMPAD4:   event = SoKeyboardEvent::NUMBER_4; break;
    case WXK_NUMPAD5:   event = SoKeyboardEvent::NUMBER_5; break;
    case WXK_NUMPAD6:   event = SoKeyboardEvent::NUMBER_6; break;
    case WXK_NUMPAD7:   event = SoKeyboardEvent::NUMBER_7; break;
    case WXK_NUMPAD8:   event = SoKeyboardEvent::NUMBER_8; break;
    case WXK_NUMPAD9:   event = SoKeyboardEvent::NUMBER_9; break;
    case WXK_MULTIPLY: break;
    case WXK_ADD: break;
    case WXK_SEPARATOR: break;
    case WXK_SUBTRACT: break;
    case WXK_DECIMAL: break;
    case WXK_DIVIDE: break;
    case WXK_F1:        event = SoKeyboardEvent::F1; break;
    case WXK_F2:        event = SoKeyboardEvent::F2; break;
    case WXK_F3:        event = SoKeyboardEvent::F3; break;
    case WXK_F4:        event = SoKeyboardEvent::F4; break;
    case WXK_F5:        event = SoKeyboardEvent::F5; break;
    case WXK_F6:        event = SoKeyboardEvent::F6; break;
    case WXK_F7:        event = SoKeyboardEvent::F7; break;
    case WXK_F8:        event = SoKeyboardEvent::F8; break;
    case WXK_F9:        event = SoKeyboardEvent::F9; break;
    case WXK_F10:       event = SoKeyboardEvent::F10; break;
    case WXK_F11:       event = SoKeyboardEvent::F11; break;
    case WXK_F12:       event = SoKeyboardEvent::F12; break;
    case WXK_F13: break;
    case WXK_F14: break;
    case WXK_F15: break;
    case WXK_F16: break;
    case WXK_F17: break;
    case WXK_F18: break;
    case WXK_F19: break;
    case WXK_F20: break;
    case WXK_F21: break;
    case WXK_F22: break;
    case WXK_F23: break;
    case WXK_F24: break;
    case WXK_NUMLOCK: break;
    case WXK_SCROLL:  break;
    case WXK_PAGEUP: break;
    case WXK_PAGEDOWN: break;
    case WXK_NUMPAD_SPACE: break;
    case WXK_NUMPAD_TAB: break;
    case WXK_NUMPAD_ENTER: break;
    case WXK_NUMPAD_F1: break;
    case WXK_NUMPAD_F2: break;
    case WXK_NUMPAD_F3: break;
    case WXK_NUMPAD_F4: break;
    case WXK_NUMPAD_HOME: break;
    case WXK_NUMPAD_LEFT: break;
    case WXK_NUMPAD_UP: break;
    case WXK_NUMPAD_RIGHT: break;
    case WXK_NUMPAD_DOWN: break;
    case WXK_NUMPAD_PRIOR: break;
    case WXK_NUMPAD_NEXT: break;
    case WXK_NUMPAD_END: break;
    case WXK_NUMPAD_BEGIN: break;
    case WXK_NUMPAD_INSERT: break;
    case WXK_NUMPAD_DELETE: break;
    case WXK_NUMPAD_EQUAL: break;
    case WXK_NUMPAD_MULTIPLY: break;
    case WXK_NUMPAD_ADD: break;
    case WXK_NUMPAD_SEPARATOR: break;
    case WXK_NUMPAD_SUBTRACT: break;
    case WXK_NUMPAD_DECIMAL: break;
    case WXK_NUMPAD_DIVIDE: break;
    }
    return event;
}


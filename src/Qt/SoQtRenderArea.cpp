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

#include <SoViewer/Qt/SoQtRenderArea.h>
#include <Inventor/SoDB.h>

#if QT_VERSION >= 0x040000
#   include <QMouseEvent>
#endif

unsigned int SoQtRenderArea::m_cache_context = 19720408;
#if QT_VERSION >= 0x040000 
SoQtRenderArea::SoQtRenderArea( QWidget * parent, const QGLWidget * shareWidget, Qt::WindowFlags f)
: QGLWidget(parent, shareWidget, f), SoRenderArea()
{
    commonInit();
}

SoQtRenderArea::SoQtRenderArea( QGLContext * context, QWidget * parent, const QGLWidget * shareWidget, Qt::WindowFlags f)
: QGLWidget(context, parent, shareWidget, f), SoRenderArea()
{
    commonInit();
}

SoQtRenderArea::SoQtRenderArea( const QGLFormat & format, QWidget * parent, const QGLWidget * shareWidget, Qt::WindowFlags f)
: QGLWidget(format, parent, shareWidget, f), SoRenderArea()
{
    commonInit();
}
#else
SoQtRenderArea::SoQtRenderArea( QWidget * parent, const char * name, const QGLWidget * shareWidget, WFlags f) 
: QGLWidget(parent, name, shareWidget, f), SoRenderArea()
{
    commonInit();  
}

SoQtRenderArea::SoQtRenderArea( QGLContext * context, QWidget * parent, const char * name, const QGLWidget * shareWidget, WFlags f )
: QGLWidget(context, parent, name, shareWidget, f), SoRenderArea()
{
    commonInit();  
}

SoQtRenderArea::SoQtRenderArea( const QGLFormat & format, QWidget * parent, const char * name, const QGLWidget * shareWidget, WFlags f)
: QGLWidget(format, parent, name, shareWidget, f), SoRenderArea()
{
    commonInit();  
}
#endif

SoQtRenderArea::~SoQtRenderArea()
{ 

}

void SoQtRenderArea::commonInit()
{
    setMouseTracking( true );

#if QT_VERSION >= 0x040000  
    setFocusPolicy(Qt::StrongFocus);
#else
    setFocusPolicy(QWidget::StrongFocus);
#endif

    m_p_scene_manager->getGLRenderAction()->setCacheContext(m_cache_context++);

    m_timerSensorId = startTimer( 1 );
    m_delaySensorId = startTimer( 0 ); //If interval is 0, then the timer event occurs once every time there are no more window system events to process.

    m_time.start();
}

void SoQtRenderArea::soRenderCallback()
{
    glDraw();
}

void SoQtRenderArea::initializeGL()
{ 
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); 

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}

void SoQtRenderArea::resizeGL(int width, int height)
{
    soResizeEvent(width, height);
}

void SoQtRenderArea::paintGL()
{
    soPaintEvent();
}

void SoQtRenderArea::timerEvent( QTimerEvent * event )
{
    if(event->timerId() == m_timerSensorId){
        SoDB::getSensorManager()->processTimerQueue();

        // Timer sensors are set up to trigger at specific, absolute times. 
        SbTime nextTimerSensor;
        if (SoDB::getSensorManager()->isTimerSensorPending(nextTimerSensor)) {
            int interval = (nextTimerSensor - SbTime::getTimeOfDay()).getMsecValue();

            killTimer(m_timerSensorId);
            m_timerSensorId = startTimer( (interval <= 0) ? 1 :  interval);
        }
    }

    if(event->timerId() == m_delaySensorId){
        // Delay sensors trigger when the application is otherwise idle.
        SoDB::getSensorManager()->processTimerQueue();
        SoDB::getSensorManager()->processDelayQueue(true);
    }
}

void SoQtRenderArea::keyPressEvent( QKeyEvent * e )
{
    m_keyboard_event->setTime(SbTime(m_time.elapsed() / 1000.0));
    m_keyboard_event->setKey( translateKey(e) );
    m_keyboard_event->setState(SoButtonEvent::DOWN);
#if QT_VERSION >= 0x040000
    m_keyboard_event->setShiftDown(e->modifiers() & Qt::ShiftModifier);
    m_keyboard_event->setCtrlDown(e->modifiers() & Qt::ControlModifier);
    m_keyboard_event->setAltDown(e->modifiers() & Qt::AltModifier);
#else
    m_keyboard_event->setShiftDown(e->state() & Qt::ShiftButton);
    m_keyboard_event->setCtrlDown(e->state() & Qt::ControlButton);
    m_keyboard_event->setAltDown(e->state() & Qt::AltButton);
#endif
    soKeyPressEvent( m_keyboard_event );

    QGLWidget::keyPressEvent( e );
}

void SoQtRenderArea::keyReleaseEvent( QKeyEvent * e )
{
    m_keyboard_event->setTime(SbTime(m_time.elapsed() / 1000.0));
    m_keyboard_event->setKey( translateKey(e) );
    m_keyboard_event->setState(SoButtonEvent::UP);
#if QT_VERSION >= 0x040000
    m_keyboard_event->setShiftDown(e->modifiers() & Qt::ShiftModifier);
    m_keyboard_event->setCtrlDown(e->modifiers() & Qt::ControlModifier);
    m_keyboard_event->setAltDown(e->modifiers() & Qt::AltModifier);
#else
    m_keyboard_event->setShiftDown(e->state() & Qt::ShiftButton);
    m_keyboard_event->setCtrlDown(e->state() & Qt::ControlButton);
    m_keyboard_event->setAltDown(e->state() & Qt::AltButton);
#endif
    soKeyReleaseEvent( m_keyboard_event );

    QGLWidget::keyReleaseEvent( e );
}

void SoQtRenderArea::mouseMoveEvent( QMouseEvent * e )
{
    m_location_event->setTime(SbTime(m_time.elapsed() / 1000.0));
    m_location_event->setPosition(SbVec2s(e->x(), height() - 1 - e->y()));
#if QT_VERSION >= 0x040000
    m_location_event->setShiftDown(e->modifiers() & Qt::ShiftModifier);
    m_location_event->setCtrlDown(e->modifiers() & Qt::ControlModifier);
    m_location_event->setAltDown(e->modifiers() & Qt::AltModifier);
#else
    m_location_event->setShiftDown(e->state() & Qt::ShiftButton);
    m_location_event->setCtrlDown(e->state() & Qt::ControlButton);
    m_location_event->setAltDown(e->state() & Qt::AltButton);
#endif
    soMouseMoveEvent( m_location_event );

    QGLWidget::mouseMoveEvent( e );
}

void SoQtRenderArea::mousePressEvent( QMouseEvent * e )
{
    switch( e->button() )
    {
    case Qt::LeftButton:
        m_mouse_button_event->setButton(SoMouseButtonEvent::BUTTON1);
        break;
    case Qt::MidButton:
        m_mouse_button_event->setButton(SoMouseButtonEvent::BUTTON2);
        break;
    case Qt::RightButton:
        m_mouse_button_event->setButton(SoMouseButtonEvent::BUTTON3);
        break;
    default:
        m_mouse_button_event->setButton(SoMouseButtonEvent::ANY); 
        break;
    } 
    m_mouse_button_event->setTime(SbTime(m_time.elapsed() / 1000.0));
    m_mouse_button_event->setState(SoButtonEvent::DOWN);
    m_mouse_button_event->setPosition(SbVec2s(e->x(), height() - 1 - e->y()));
#if QT_VERSION >= 0x040000 
    m_mouse_button_event->setShiftDown(e->modifiers() & Qt::ShiftModifier);
    m_mouse_button_event->setCtrlDown(e->modifiers() & Qt::ControlModifier);
    m_mouse_button_event->setAltDown(e->modifiers() & Qt::AltModifier);
#else  
    m_mouse_button_event->setShiftDown(e->state() & Qt::ShiftButton);
    m_mouse_button_event->setCtrlDown(e->state() & Qt::ControlButton);
    m_mouse_button_event->setAltDown(e->state() & Qt::AltButton);
#endif     
    soMousePressEvent( m_mouse_button_event );

    QGLWidget::mousePressEvent( e );
}

void SoQtRenderArea::mouseReleaseEvent( QMouseEvent * e )
{
    switch( e->button() )
    {
#if QT_VERSION >= 0x040000
  case Qt::LeftButton:
      m_mouse_button_event->setButton(SoMouseButtonEvent::BUTTON1);
      break;
  case Qt::MidButton:
      m_mouse_button_event->setButton(SoMouseButtonEvent::BUTTON2);
      break;
  case Qt::RightButton:
      m_mouse_button_event->setButton(SoMouseButtonEvent::BUTTON3);
      break;
#else
  case QMouseEvent::LeftButton:
      m_mouse_button_event->setButton(SoMouseButtonEvent::BUTTON1);
      break;
  case QMouseEvent::MidButton:
      m_mouse_button_event->setButton(SoMouseButtonEvent::BUTTON2);
      break;
  case QMouseEvent::RightButton:
      m_mouse_button_event->setButton(SoMouseButtonEvent::BUTTON3);
      break;
#endif
  default:
      m_mouse_button_event->setButton(SoMouseButtonEvent::ANY); 
      break;  
    } 
    m_mouse_button_event->setTime(SbTime(m_time.elapsed() / 1000.0));
    m_mouse_button_event->setState(SoButtonEvent::UP);
    m_mouse_button_event->setPosition(SbVec2s(e->x(), height() - 1 - e->y()));
#if QT_VERSION >= 0x040000
    m_mouse_button_event->setShiftDown(e->modifiers() & Qt::ShiftModifier);
    m_mouse_button_event->setCtrlDown(e->modifiers() & Qt::ControlModifier);
    m_mouse_button_event->setAltDown(e->modifiers() & Qt::AltModifier);
#else
    m_mouse_button_event->setShiftDown(e->state() & Qt::ShiftButton);
    m_mouse_button_event->setCtrlDown(e->state() & Qt::ControlButton);
    m_mouse_button_event->setAltDown(e->state() & Qt::AltButton);
#endif  
    soMouseReleaseEvent( m_mouse_button_event );

    QGLWidget::mouseReleaseEvent( e );
}

void SoQtRenderArea::wheelEvent( QWheelEvent * e )
{
    m_wheel_event->setTime(SbTime(m_time.elapsed() / 1000.0));
    m_wheel_event->setPosition(SbVec2s(e->x(), height() - 1 - e->y()));
#if QT_VERSION >= 0x040000
    m_wheel_event->setShiftDown(e->modifiers() & Qt::ShiftModifier);
    m_wheel_event->setCtrlDown(e->modifiers() & Qt::ControlModifier);
    m_wheel_event->setAltDown(e->modifiers() & Qt::AltModifier);
#else
    m_wheel_event->setShiftDown(e->state() & Qt::ShiftButton);
    m_wheel_event->setCtrlDown(e->state() & Qt::ControlButton);
    m_wheel_event->setAltDown(e->state() & Qt::AltButton);
#endif
    m_wheel_event->setDelta(e->delta());
    soWheelEvent( m_wheel_event );

    QGLWidget::wheelEvent( e );
}

SoKeyboardEvent::Key SoQtRenderArea::translateKey( QKeyEvent * e )
{
    SoKeyboardEvent::Key event = SoKeyboardEvent::ANY;

    switch( e->key() )
    {
    case Qt::Key_Escape: event = SoKeyboardEvent::ESCAPE; break;
    case Qt::Key_Tab: event = SoKeyboardEvent::TAB; break;
    case Qt::Key_Backspace: event = SoKeyboardEvent::BACKSPACE; break;
    case Qt::Key_Return: event = SoKeyboardEvent::RETURN; break;
    case Qt::Key_Enter: event = SoKeyboardEvent::ENTER; break;
    case Qt::Key_Insert: event = SoKeyboardEvent::INSERT; break;
        // Avoid problem with Microsoft Visual C++ Win32 API headers (yes,
        // they actually #define DELETE in their WINNT.H header file).
#ifdef DELETE
    case Qt::Key_Delete: event = SoKeyboardEvent::KEY_DELETE; break;
#else
    case Qt::Key_Delete: event = SoKeyboardEvent::DELETE; break;
#endif
    case Qt::Key_Pause: event = SoKeyboardEvent::PAUSE; break;
    case Qt::Key_Print: event = SoKeyboardEvent::PRINT; break;
    case Qt::Key_Home: event = SoKeyboardEvent::HOME; break;
    case Qt::Key_End: event = SoKeyboardEvent::END; break;
    case Qt::Key_Left: event = SoKeyboardEvent::LEFT_ARROW; break;
    case Qt::Key_Up: event = SoKeyboardEvent::UP_ARROW; break;
    case Qt::Key_Right: event = SoKeyboardEvent::RIGHT_ARROW; break;
    case Qt::Key_Down: event = SoKeyboardEvent::DOWN_ARROW; break;
    case Qt::Key_PageUp: event = SoKeyboardEvent::PAGE_UP; break;
    case Qt::Key_PageDown: event = SoKeyboardEvent::PAGE_DOWN; break;
    case Qt::Key_Shift: event = SoKeyboardEvent::LEFT_SHIFT; break;
    case Qt::Key_Control: event = SoKeyboardEvent::LEFT_CONTROL; break;
    case Qt::Key_Meta: event = SoKeyboardEvent::LEFT_ALT; break;
    case Qt::Key_Alt: event = SoKeyboardEvent::LEFT_ALT; break;
    case Qt::Key_CapsLock: event = SoKeyboardEvent::CAPS_LOCK; break;
    case Qt::Key_NumLock: event = SoKeyboardEvent::NUM_LOCK; break;
    case Qt::Key_ScrollLock: event = SoKeyboardEvent::SCROLL_LOCK; break;
    case Qt::Key_F1: event = SoKeyboardEvent::F1; break;
    case Qt::Key_F2: event = SoKeyboardEvent::F2; break;
    case Qt::Key_F3: event = SoKeyboardEvent::F3; break;
    case Qt::Key_F4: event = SoKeyboardEvent::F4; break;
    case Qt::Key_F5: event = SoKeyboardEvent::F5; break;
    case Qt::Key_F6: event = SoKeyboardEvent::F6; break;
    case Qt::Key_F7: event = SoKeyboardEvent::F7; break;
    case Qt::Key_F8: event = SoKeyboardEvent::F8; break;
    case Qt::Key_F9: event = SoKeyboardEvent::F9; break;
    case Qt::Key_F10: event = SoKeyboardEvent::F10; break;
    case Qt::Key_F11: event = SoKeyboardEvent::F11; break;
    case Qt::Key_F12: event = SoKeyboardEvent::F12; break;
    case Qt::Key_Space: event = SoKeyboardEvent::SPACE; break;
    case Qt::Key_Exclam: event = SoKeyboardEvent::NUMBER_1; break;
    case Qt::Key_QuoteDbl: event = SoKeyboardEvent::APOSTROPHE; break;
    case Qt::Key_NumberSign: event = SoKeyboardEvent::NUMBER_3; break;
    case Qt::Key_Dollar: event = SoKeyboardEvent::NUMBER_4; break;
    case Qt::Key_Percent: event = SoKeyboardEvent::NUMBER_5; break;
    case Qt::Key_Ampersand: event = SoKeyboardEvent::NUMBER_6; break;
    case Qt::Key_Apostrophe: event = SoKeyboardEvent::APOSTROPHE; break;
    case Qt::Key_ParenLeft: event = SoKeyboardEvent::NUMBER_9; break;
    case Qt::Key_ParenRight: event = SoKeyboardEvent::NUMBER_0; break;
    case Qt::Key_Asterisk: event = SoKeyboardEvent::NUMBER_8; break;
    case Qt::Key_Plus: event = SoKeyboardEvent::PAD_ADD; break;
    case Qt::Key_Minus: event = SoKeyboardEvent::PAD_SUBTRACT; break;
    case Qt::Key_Comma: event = SoKeyboardEvent::COMMA; break;
    case Qt::Key_Period: event = SoKeyboardEvent::PERIOD; break;
    case Qt::Key_Slash: event = SoKeyboardEvent::SLASH; break;
    case Qt::Key_0: event = SoKeyboardEvent::NUMBER_0; break;
    case Qt::Key_1: event = SoKeyboardEvent::NUMBER_1; break;
    case Qt::Key_2: event = SoKeyboardEvent::NUMBER_2; break;
    case Qt::Key_3: event = SoKeyboardEvent::NUMBER_3; break;
    case Qt::Key_4: event = SoKeyboardEvent::NUMBER_4; break;
    case Qt::Key_5: event = SoKeyboardEvent::NUMBER_5; break;
    case Qt::Key_6: event = SoKeyboardEvent::NUMBER_6; break;
    case Qt::Key_7: event = SoKeyboardEvent::NUMBER_7; break;
    case Qt::Key_8: event = SoKeyboardEvent::NUMBER_8; break;
    case Qt::Key_9: event = SoKeyboardEvent::NUMBER_9; break;
    case Qt::Key_Colon: event = SoKeyboardEvent::SEMICOLON; break;
    case Qt::Key_Semicolon: event = SoKeyboardEvent::SEMICOLON; break;
    case Qt::Key_Less: event = SoKeyboardEvent::COMMA; break;
    case Qt::Key_Equal: event = SoKeyboardEvent::EQUAL; break;
    case Qt::Key_Greater: event = SoKeyboardEvent::PERIOD; break;
    case Qt::Key_Question: event = SoKeyboardEvent::BACKSLASH; break;
    case Qt::Key_At: event = SoKeyboardEvent::NUMBER_2; break;
    case Qt::Key_A: event = SoKeyboardEvent::A; break;
    case Qt::Key_B: event = SoKeyboardEvent::B; break;
    case Qt::Key_C: event = SoKeyboardEvent::C; break;
    case Qt::Key_D: event = SoKeyboardEvent::D; break;
    case Qt::Key_E: event = SoKeyboardEvent::E; break;
    case Qt::Key_F: event = SoKeyboardEvent::F; break;
    case Qt::Key_G: event = SoKeyboardEvent::G; break;
    case Qt::Key_H: event = SoKeyboardEvent::H; break;
    case Qt::Key_I: event = SoKeyboardEvent::I; break;
    case Qt::Key_J: event = SoKeyboardEvent::J; break;
    case Qt::Key_K: event = SoKeyboardEvent::K; break;
    case Qt::Key_L: event = SoKeyboardEvent::L; break;
    case Qt::Key_M: event = SoKeyboardEvent::M; break;
    case Qt::Key_N: event = SoKeyboardEvent::N; break;
    case Qt::Key_O: event = SoKeyboardEvent::O; break;
    case Qt::Key_P: event = SoKeyboardEvent::P; break;
    case Qt::Key_Q: event = SoKeyboardEvent::Q; break;
    case Qt::Key_R: event = SoKeyboardEvent::R; break;
    case Qt::Key_S: event = SoKeyboardEvent::S; break;
    case Qt::Key_T: event = SoKeyboardEvent::T; break;
    case Qt::Key_U: event = SoKeyboardEvent::U; break;
    case Qt::Key_V: event = SoKeyboardEvent::V; break;
    case Qt::Key_W: event = SoKeyboardEvent::W; break;
    case Qt::Key_X: event = SoKeyboardEvent::X; break;
    case Qt::Key_Y: event = SoKeyboardEvent::Y; break;
    case Qt::Key_Z: event = SoKeyboardEvent::Z; break;
    case Qt::Key_BracketLeft: event = SoKeyboardEvent::BRACKETLEFT; break;
    case Qt::Key_Backslash: event = SoKeyboardEvent::BACKSLASH; break;
    case Qt::Key_BracketRight: event = SoKeyboardEvent::BRACKETRIGHT; break;
    case Qt::Key_AsciiCircum: event = SoKeyboardEvent::NUMBER_7; break;
    case Qt::Key_Underscore: event = SoKeyboardEvent::MINUS; break;
    case Qt::Key_BraceLeft: event = SoKeyboardEvent::BRACKETLEFT; break;
    case Qt::Key_Bar: event = SoKeyboardEvent::BACKSLASH; break;
    case Qt::Key_BraceRight: event = SoKeyboardEvent::BRACKETRIGHT; break;
    case Qt::Key_AsciiTilde: event = SoKeyboardEvent::GRAVE; break;
    case Qt::Key_unknown: event = SoKeyboardEvent::ANY; break;
    }
    return event;
}

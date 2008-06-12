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
#ifndef _SOWXRENDERAREA_H_
#define _SOWXRENDERAREA_H_

#include <wx/glcanvas.h>
#include <wx/timer.h>

#include <SoViewer/SoRenderArea.h>

class SOVIEWER_API SoWxRenderArea : public wxGLCanvas, public SoRenderArea
{
public:
    SoWxRenderArea(wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = 0,
        const wxString& name = wxT("SoWxRenderArea"));
    virtual ~SoWxRenderArea();

protected:
    virtual void soRenderCallback();

    void initializeGL();
    void OnTimerEvent(wxTimerEvent& event);
    void OnIdleEvent(wxIdleEvent& event);
    void OnKeyDown(wxKeyEvent& event);
    void OnKeyUp(wxKeyEvent& event); 
    void OnMousePress(wxMouseEvent& event);
    void OnMouseMove(wxMouseEvent& event);
    void OnMouseRelease(wxMouseEvent& event);
    void OnMouseWheel(wxMouseEvent& event);
    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnEraseBackground(wxEraseEvent& event);
private:
    wxTimer				 m_time;

    SoKeyboardEvent::Key translateKey( wxKeyEvent& event );

    DECLARE_EVENT_TABLE()
};

#endif // _SOWXRENDERAREA_H_

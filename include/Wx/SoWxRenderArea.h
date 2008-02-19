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
#ifndef _SOWXRENDERAREA_H_
#define _SOWXRENDERAREA_H_

#include <wx/glcanvas.h>
#include <wx/timer.h>

#include <SoRenderArea.h>

class SoWxRenderArea : public wxGLCanvas, public SoRenderArea
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
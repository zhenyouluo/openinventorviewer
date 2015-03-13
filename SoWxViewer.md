# Introduction #

wxWidget 2.x version of the viewer and the render area.

```
#include <wx/wx.h>
#include <wx/glcanvas.h>

#include <SoViewer/Wx/SoWxViewer.h>

#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/manips/SoTrackballManip.h>

// Define a new application type
class MyApp: public wxApp
{
public:
	bool OnInit()
	{
                SoDB::init();
                SoNodeKit::init();
                SoInteraction::init();

		// Create the main frame window
		wxFrame *frame = new wxFrame(NULL, wxID_ANY, _T("wxWidgets OpenInventor Sample"), wxDefaultPosition, wxSize(400, 300));

		SoWxViewer * myViewer = new SoWxViewer(frame, wxID_ANY);
		
		SoSeparator * root = new SoSeparator;
		root->ref();

		root->addChild( new SoTrackballManip );
		root->addChild( new SoCube );

		// Show the frame
		frame->Show(true);

		myViewer->setBackgroundColor(SbColor(0.68f,0.7f,0.8f));
		myViewer->setSceneGraph(root);
		myViewer->viewAll();

		return true;
	}

};

IMPLEMENT_APP(MyApp)
```
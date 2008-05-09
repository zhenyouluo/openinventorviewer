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
#include <SoViewer/Wx/SoWxViewer.h>

#include <wx/bitmap.h>
#include <wx/image.h>

#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/actions/SoSearchAction.h>

#include <Inventor/projectors/SbSphereSheetProjector.h>

SoWxViewer::SoWxViewer(wxWindow *parent, wxWindowID id,
                       const wxPoint& pos, const wxSize& size, long style, const wxString& name)
                       : SoWxRenderArea(parent, id, pos, size, style, name),
                       m_p_sceneGraph(0),
                       m_p_camera(0),
                       m_viewing(true)
{
    m_mouseButton = SoMouseButtonEvent::ANY;
    m_p_rootGraph = new SoSeparator;
    m_p_rootGraph->ref();

    m_p_directionalLight = new SoDirectionalLight;
    m_p_directionalLight->ref();

    m_cameraType = SoPerspectiveCamera::getClassTypeId();

    setViewing(true);

    SoWxRenderArea::setSceneGraph(m_p_rootGraph);
}

SoWxViewer::~SoWxViewer()
{
    m_p_rootGraph->unref();
    m_p_directionalLight->unref();
}

void SoWxViewer::setCamera(SoCamera *camera)
{
    if(!camera || m_p_camera == camera) return;

    if(m_p_camera) m_p_rootGraph->replaceChild(m_p_camera, camera);

    m_p_camera = camera;
}

void SoWxViewer::setCameraType(SoType mType)
{
    if(mType.isDerivedFrom(SoPerspectiveCamera::getClassTypeId()) ||
        mType.isDerivedFrom(SoOrthographicCamera::getClassTypeId()))
    {
        m_cameraType = mType;
    }
}

SoCamera * SoWxViewer::getCamera()
{
    return m_p_camera;
}

void SoWxViewer::setSceneGraph(SoNode * a_new_scene)
{
    m_p_rootGraph->removeAllChildren();

    SoSearchAction sa;
    sa.setType(SoCamera::getClassTypeId());
    sa.setSearchingAll(false);
    sa.apply(m_p_rootGraph);

    if (sa.getPath()){
        m_p_camera = (SoCamera*)((SoFullPath *)sa.getPath())->getTail();
        m_cameraType = m_p_camera->getTypeId();
    }else{
        if(m_cameraType == SoPerspectiveCamera::getClassTypeId())
            m_p_camera = new SoPerspectiveCamera;
        else
            m_p_camera = new SoOrthographicCamera;

        m_p_rootGraph->addChild(m_p_camera);
    }
    m_p_rootGraph->addChild(m_p_directionalLight);
    m_p_rootGraph->addChild(a_new_scene);

    m_p_sceneGraph = a_new_scene;
}

SoNode * SoWxViewer::getSceneGraph()
{
    return m_p_sceneGraph;
}

void SoWxViewer::viewAll()
{
    if(!m_p_camera) return;

    m_p_camera->viewAll(m_p_rootGraph, getViewportRegion());
}

void SoWxViewer::setViewing(bool state)
{
    m_viewing = state;

    static const unsigned char openhand_bits[] = 
    {
        0x80,0x01,0x58,0x0e,0x64,0x12,0x64,0x52,0x48,0xb2,0x48,0x92,
        0x16,0x90,0x19,0x80,0x11,0x40,0x02,0x40,0x04,0x40,0x04,0x20,
        0x08,0x20,0x10,0x10,0x20,0x10,0x00,0x00
    };
    static const unsigned char openhandm_bits[] = 
    {
        0x7f,0xfe,0x27,0xf0,0x03,0xe0,0x03,0xa0,0x07,0x00,0x07,0x00,
        0x01,0x00,0x00,0x00,0x00,0x80,0x01,0x80,0x03,0x80,0x03,0xc0,
        0x07,0xc0,0x0f,0xe0,0x1f,0xe0,0xff,0xff
    };
    static const unsigned char closedhand_bits[] = 
    {
        0x00,0x00,0x00,0x00,0x00,0x00,0xb0,0x0d,0x48,0x32,0x08,0x50,
        0x10,0x40,0x18,0x40,0x04,0x40,0x04,0x20,0x08,0x20,0x10,0x10,
        0x20,0x10,0x20,0x10,0x00,0x00,0x00,0x00
    };
    static const unsigned char closedhandm_bits[] = 
    {
        0xff,0xff,0xff,0xff,0xff,0xff,0x4f,0xf2,0x07,0xc0,0x07,0x80,
        0x0f,0x80,0x07,0x80,0x03,0x80,0x03,0xc0,0x07,0xc0,0x0f,0xe0,
        0x1f,0xe0,0x1f,0xe0,0xff,0xff,0xff,0xff
    };

    if(m_viewing){
        const unsigned char * cursor_bits = (m_mouseButton == SoMouseButtonEvent::ANY) ? openhand_bits : closedhand_bits;
        const unsigned char * mask_bits = (m_mouseButton == SoMouseButtonEvent::ANY) ? openhandm_bits : closedhandm_bits;

        wxBitmap cbitmap((char*)cursor_bits,16,16);
        wxBitmap mbitmap((char*)mask_bits,16,16);
        cbitmap.SetMask(new wxMask(mbitmap));

        SetCursor(cbitmap.ConvertToImage());
    }else SetCursor(wxCURSOR_ARROW);
}

SbVec3f SoWxViewer::projectPoint(const SbVec2f & pt) const
{
    if(!m_p_camera) return SbVec3f(0.0f, 0.0f, 0.0f);

    wxSize wxsize = wxGLCanvas::GetClientSize();

    // Avoid problems when width < height.
    SbViewVolume cameraVolume;
    if(m_p_camera->viewportMapping.getValue() == SoCamera::ADJUST_CAMERA){ 
        float aspectRatio = (wxsize.GetHeight() == 0) ? 1.0f : (wxsize.GetWidth() / (float)wxsize.GetHeight());

        cameraVolume = m_p_camera->getViewVolume(aspectRatio); 
        if (aspectRatio < 1.0) cameraVolume.scale(1.0f / aspectRatio); 
    } 
    else cameraVolume = m_p_camera->getViewVolume(0.0f);    

    // Map screen location into the camera focal plane
    return cameraVolume.getPlanePoint(m_p_camera->focalDistance.getValue(), pt);
}


void SoWxViewer::rotateCamera(const SbRotation &rot)
{
    if (!m_p_camera) return;

    // get center of rotation
    float radius = m_p_camera->focalDistance.getValue();

    SbVec3f forward;
    m_p_camera->orientation.getValue().multVec(SbVec3f(0,0,-1), forward);

    SbVec3f center = m_p_camera->position.getValue() + radius * forward;

    // apply new rotation to the camera
    m_p_camera->orientation = rot * m_p_camera->orientation.getValue();

    // reposition camera to look at pt of interest
    m_p_camera->orientation.getValue().multVec(SbVec3f(0,0,-1), forward);
    m_p_camera->position = center - radius * forward;
}

void SoWxViewer::soPaintEvent()
{
    adjustCameraClippingPlanes();

    if(m_p_camera){
        SbVec3f forward;
        m_p_camera->orientation.getValue().multVec(SbVec3f(0,0,-1), forward);

        m_p_directionalLight->direction = forward;
    }
    SoWxRenderArea::soPaintEvent();
}

void SoWxViewer::soKeyPressEvent( SoKeyboardEvent * e)
{
    if(e->getKey() == SoKeyboardEvent::ESCAPE) setViewing(!m_viewing);

    SoWxRenderArea::soKeyPressEvent( e );
}

void SoWxViewer::soMousePressEvent( SoMouseButtonEvent * e )
{
    if(!m_viewing) SoWxRenderArea::soMousePressEvent(e);

    m_mouseButton = e->getButton();
    m_startDrag = e->getNormalizedPosition(getViewportRegion());

    setViewing(m_viewing);
}

void SoWxViewer::soMouseReleaseEvent( SoMouseButtonEvent * e )
{
    if(!m_viewing) SoWxRenderArea::soMouseReleaseEvent(e);

    m_mouseButton = SoMouseButtonEvent::ANY;

    setViewing(m_viewing);
}

void SoWxViewer::soMouseMoveEvent( SoLocation2Event * e )
{
    if(!m_viewing) SoWxRenderArea::soMouseMoveEvent(e);

    if (!m_p_camera || !m_viewing) return;

    //SbVec2f endDrag(e->x()/(float)width(), (height() - e->y())/(float)height());
    SbVec2f endDrag = e->getNormalizedPosition(getViewportRegion());

    if(m_mouseButton == SoMouseButtonEvent::BUTTON1){
        // Init the sphere projector class
        SbViewVolume vv;
        vv.ortho(-1, 1, -1, 1, -10, 10);
        SbSphereSheetProjector sphereSheet;
        sphereSheet.setViewVolume( vv );
        sphereSheet.setSphere( SbSphere( SbVec3f(0, 0, 0), .7f) );

        // Set the sphere sheet starting point
        SbVec3f lstart = sphereSheet.project(m_startDrag);
        SbVec3f lend = sphereSheet.project(endDrag);

        rotateCamera( sphereSheet.getRotation(lstart, lend).inverse() );
    }

    if(m_mouseButton == SoMouseButtonEvent::BUTTON2){
        SbVec3f lstart = projectPoint(m_startDrag);
        SbVec3f lend = projectPoint(endDrag);

        // move the camera by the delta 3D position amount
        m_p_camera->position = m_p_camera->position.getValue() +  (lstart - lend);
    }

    m_startDrag = endDrag;
}

void SoWxViewer::soWheelEvent( SoWheelEvent * e )
{
    if (!m_p_camera || !m_viewing) return;

    float dol = pow(2.0f,e->getDelta() / 4000.0f);

    if (m_p_camera->isOfType(SoOrthographicCamera::getClassTypeId())) {
        // change the ortho camera height
        SoOrthographicCamera *cam = (SoOrthographicCamera *) m_p_camera;
        cam->height = cam->height.getValue() * dol;
    }else{
        // shorter/grow the focal distance given the mouse move
        float focalDistance = m_p_camera->focalDistance.getValue();;
        float newFocalDist = focalDistance * dol;

        // finally reposition the camera
        SbVec3f forward;
        m_p_camera->orientation.getValue().multVec(SbVec3f(0,0,-1), forward);
        m_p_camera->position = m_p_camera->position.getValue() + (focalDistance - newFocalDist) * forward;
        m_p_camera->focalDistance = newFocalDist;
    }
}

void SoWxViewer::adjustCameraClippingPlanes()
{
    if (!m_p_camera) return;

    SoGetBoundingBoxAction clipbox_action(getViewportRegion());
    clipbox_action.apply(m_p_rootGraph);

    SbBox3f bbox = clipbox_action.getBoundingBox();

    SbSphere bSphere;
    bSphere.circumscribe(bbox);

    SbVec3f forward;
    m_p_camera->orientation.getValue().multVec(SbVec3f(0,0,-1), forward);

    float denumerator = forward.length();
    float numerator = (bSphere.getCenter() - m_p_camera->position.getValue()).dot(forward);
    float distToCenter = (forward * (numerator / denumerator)).length();

    float farplane = distToCenter + bSphere.getRadius();

    // if scene is behind the camera, don't change the planes
    if (farplane < 0) return;

    float nearplane = distToCenter - bSphere.getRadius();

    if (nearplane < (0.001 * farplane)) nearplane = 0.001 * farplane;

    m_p_camera->nearDistance = nearplane;
    m_p_camera->farDistance = farplane;
}

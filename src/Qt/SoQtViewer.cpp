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
#include <SoViewer/Qt/SoQtViewer.h>

#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/actions/SoSearchAction.h>

#include <Inventor/projectors/SbSphereSheetProjector.h>

#if QT_VERSION < 0x040000
#   include <qbitmap.h>
#   include <qcursor.h> 
#endif

SoQtViewer::SoQtViewer(QWidget *parent)
: SoQtRenderArea(parent),
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

    SoQtRenderArea::setSceneGraph(m_p_rootGraph);
}

SoQtViewer::~SoQtViewer()
{
    m_p_rootGraph->unref();
    m_p_directionalLight->unref();
}

void SoQtViewer::setCamera(SoCamera *camera)
{
    if(!camera || m_p_camera == camera) return;

    if(m_p_camera) m_p_rootGraph->replaceChild(m_p_camera, camera);

    m_p_camera = camera;
}

void SoQtViewer::setCameraType(SoType mType)
{
    if(mType.isDerivedFrom(SoPerspectiveCamera::getClassTypeId()) ||
        mType.isDerivedFrom(SoOrthographicCamera::getClassTypeId()))
    {
        m_cameraType = mType;
    }
}

SoCamera * SoQtViewer::getCamera()
{
    return m_p_camera;
}

void SoQtViewer::setSceneGraph(SoNode * a_new_scene)
{
    m_p_rootGraph->removeAllChildren();

    SoSearchAction sa;
    sa.setType(SoCamera::getClassTypeId());
    sa.setSearchingAll(false);
    sa.apply(a_new_scene);

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

SoNode * SoQtViewer::getSceneGraph()
{
    return m_p_sceneGraph;
}

void SoQtViewer::viewAll()
{
    if(!m_p_camera) return;

    m_p_camera->viewAll(m_p_rootGraph, getViewportRegion());
}

void SoQtViewer::setViewing(bool state)
{
    m_viewing = state;

#if QT_VERSION >= 0x040000
    if(m_viewing) setCursor((m_mouseButton == SoMouseButtonEvent::ANY) ? Qt::OpenHandCursor : Qt::ClosedHandCursor);
    else setCursor(Qt::ArrowCursor);
#else
    static const unsigned char openhand_bits[] = 
    {
        0x80,0x01,0x58,0x0e,0x64,0x12,0x64,0x52,0x48,0xb2,0x48,0x92,
        0x16,0x90,0x19,0x80,0x11,0x40,0x02,0x40,0x04,0x40,0x04,0x20,
        0x08,0x20,0x10,0x10,0x20,0x10,0x00,0x00
    };
    // WARNING ! Masks are inverted compared to WxWidget
    static const unsigned char openhandm_bits[] = 
    {
        0x80,0x01,0xd8,0x0f,0xfc,0x1f,0xfc,0x5f,0xf8,0xff,0xf8,0xff,
        0xfe,0xff,0xff,0xff,0xff,0x7f,0xfe,0x7f,0xfc,0x7f,0xfc,0x3f,
        0xf8,0x3f,0xf0,0x1f,0xe0,0x1f,0x00,0x0
    };
    static const unsigned char closedhand_bits[] = 
    {
        0x00,0x00,0x00,0x00,0x00,0x00,0xb0,0x0d,0x48,0x32,0x08,0x50,
        0x10,0x40,0x18,0x40,0x04,0x40,0x04,0x20,0x08,0x20,0x10,0x10,
        0x20,0x10,0x20,0x10,0x00,0x00,0x00,0x00
    };

    static const unsigned char closedhandm_bits[] = 
    {
        0x00,0x00,0x00,0x00,0x00,0x00,0xb0,0x0d,0xf8,0x3f,0xf8,0x7f,
        0xf0,0x7f,0xf8,0x7f,0xfc,0x7f,0xfc,0x3f,0xf8,0x3f,0xf0,0x1f,
        0xe0,0x1f,0xe0,0x1f,0x00,0x00,0x00,0x00
    };

    static QCursor OpenHandCursor = QCursor(QBitmap(16, 16, openhand_bits, true), QBitmap(16, 16, openhandm_bits, true));
    static QCursor ClosedHandCursor = QCursor(QBitmap(16, 16, closedhand_bits, true), QBitmap(16, 16, closedhandm_bits, true));

    if(m_viewing) setCursor((m_mouseButton == SoMouseButtonEvent::ANY) ? OpenHandCursor : ClosedHandCursor);
    else setCursor(Qt::ArrowCursor);
#endif
}

SbVec3f SoQtViewer::projectPoint(const SbVec2f & pt) const
{
    if(!m_p_camera) return SbVec3f(0.0f, 0.0f, 0.0f);

    // Avoid problems when width < height.
    SbViewVolume cameraVolume;
    if(m_p_camera->viewportMapping.getValue() == SoCamera::ADJUST_CAMERA){ 
        float aspectRatio = (height() == 0) ? 1.0f : (width() / (float)height());

        cameraVolume = m_p_camera->getViewVolume(aspectRatio); 
        if (aspectRatio < 1.0) cameraVolume.scale(1.0f / aspectRatio); 
    } 
    else cameraVolume = m_p_camera->getViewVolume(0.0f);    

    // Map screen location into the camera focal plane
    return cameraVolume.getPlanePoint(m_p_camera->focalDistance.getValue(), pt);
}


void SoQtViewer::rotateCamera(const SbRotation &rot)
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

void SoQtViewer::soPaintEvent()
{
    adjustCameraClippingPlanes();

    if(m_p_camera){
        SbVec3f forward;
        m_p_camera->orientation.getValue().multVec(SbVec3f(0,0,-1), forward);

        m_p_directionalLight->direction = forward;
    }
    SoQtRenderArea::soPaintEvent();
}

void SoQtViewer::soKeyPressEvent( SoKeyboardEvent * e)
{
    if(e->getKey() == SoKeyboardEvent::ESCAPE) setViewing(!m_viewing);

    SoQtRenderArea::soKeyPressEvent( e );
}

void SoQtViewer::soMousePressEvent( SoMouseButtonEvent * e )
{
    if(!m_viewing) SoQtRenderArea::soMousePressEvent(e);

    m_mouseButton = e->getButton();
    m_startDrag = e->getNormalizedPosition(getViewportRegion());

    setViewing(m_viewing);
}

void SoQtViewer::soMouseReleaseEvent( SoMouseButtonEvent * e )
{
    if(!m_viewing) SoQtRenderArea::soMouseReleaseEvent(e);

    m_mouseButton = SoMouseButtonEvent::ANY;

    setViewing(m_viewing);
}

void SoQtViewer::soMouseMoveEvent( SoLocation2Event * e )
{
    if(!m_viewing) SoQtRenderArea::soMouseMoveEvent(e);

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

void SoQtViewer::soWheelEvent( SoWheelEvent * e )
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

void SoQtViewer::adjustCameraClippingPlanes()
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

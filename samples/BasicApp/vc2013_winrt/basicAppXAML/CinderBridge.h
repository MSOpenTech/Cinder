// Cinder bridge to XAML
// zv new file

#pragma once

#include "cinder/Cinder.h"

#include "cinder/app/AppBasic.h"
#include "cinder/app/RendererDx.h"
#include "cinder/dx/dx.h"
#include "cinder/Surface.h"
#include "cinder/dx/DxTexture.h"

#include "cinder/app/AppImplMswRendererDx.h"

#include "cinder/Vector.h"
#include "cinder/app/MouseEvent.h"
#include "cinder/app/KeyEvent.h"

// for PointerEventArgs
using namespace Windows::UI::Core;

// fwd decls for the WinRT XAML framework
//
namespace DX {
    class DeviceResources;
}


namespace cinder { namespace app {

class AppBasicXAML 
{
public:

    AppBasicXAML();
    // AppBasicXAML(const std::shared_ptr<DX::DeviceResources>& deviceResources);
    ~AppBasicXAML();

    // Cinder side:
    // these fns can be overloaded by the Cinder BasicApp
    virtual void mouseDrag( MouseEvent event ) {}
    virtual void keyDown( KeyEvent event ) {}
    virtual void update() {}
    virtual void draw() {}

    // setFullScreen: n/a: a Windows RT store app is always full screen
    //
    // multitouch: TBD
    // void enableMultiTouch( bool enable = true ) { mEnableMultiTouch = enable; }
    // 
    // cursor: does not apply
    //
    // framerate: changing it is useful, not implemented now
    //
    // quit: TBD, stop the RenderLoop
    //  nb. WinRT OS kills the app when it needs to, otherwise the app always runs
    //  nb. RenderLoop is automatically paused when the app is not visible, by XAML framework

    // DX / XAML framework side:
    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();
    void ReleaseDeviceDependentResources();
    // void Update(DX::StepTimer const& timer);
    void Render();

    /// share the DX/D3D/D2D objects with Cinder
    void shareWithCinder();

    // pointer interface
    void StartTracking() { m_tracking = true; }
    void TrackingUpdate(PointerEventArgs^ e);
    void StopTracking() { m_tracking = false; }
    bool IsTracking() { return m_tracking; }

    // key interface TBD

  	// Cached pointer to device resources from XAML framework
	std::shared_ptr<DX::DeviceResources> m_deviceResources;

    // zv todo make this a smart ptr
    // std::shared_ptr<cinder::app::AppImplMswRendererDx> ren;
    cinder::app::AppImplMswRendererDx *ren;
private:
    bool    m_tracking;
    bool    m_pipeline_ready;
};

}}


// nb. a global base class ptr is declared and set, but the derived class is instantiated
#undef CINDER_APP_BASIC
#define CINDER_APP_BASIC( APP, RENDERER ) \
	AppBasicXAML *app = new APP;													


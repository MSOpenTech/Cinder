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

// fwd decl
class BasicApp;

namespace cinder { namespace app {

class AppBasicXAML 
{
public:

    AppBasicXAML();
    // AppBasicXAML(const std::shared_ptr<DX::DeviceResources>& deviceResources);
    ~AppBasicXAML();

    // Cinder side:
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
    void Update(DX::StepTimer const& timer);
    void Render();

    // pointer interface
    void StartTracking() {}
    void TrackingUpdate(float positionX) {}
    void StopTracking() {}
    bool IsTracking() { return false; } // return m_tracking

    // key interface TBD

  	// Cached pointer to device resources from XAML framework
	std::shared_ptr<DX::DeviceResources> m_deviceResources;
    // std::shared_ptr<cinder::app::AppImplMswRendererDx> ren;
    cinder::app::AppImplMswRendererDx *ren;
};

}}


#undef CINDER_APP_BASIC
#define CINDER_APP_BASIC( APP, RENDERER ) \
	BasicApp *app = new APP;													


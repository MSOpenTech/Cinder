// Cinder bridge to XAML
// zv new file

#pragma once

#include "cinder/Cinder.h"

#include "cinder/app/AppBasic.h"
#include "cinder/app/RendererDx.h"
#include "cinder/dx/dx.h"
#include "cinder/Surface.h"
#include "cinder/dx/DxTexture.h"

#include "cinder/Vector.h"
#include "cinder/app/MouseEvent.h"
#include "cinder/app/KeyEvent.h"

//using namespace ci;
//using namespace ci::app;

namespace cinder { namespace app {


class AppBasicXAML 
{
public:
    AppBasicXAML(const std::shared_ptr<DX::DeviceResources>& deviceResources) : 
        m_deviceResources(deviceResources) {};

    // Cinder side:
    virtual void mouseDrag( MouseEvent event ) {}
    virtual void keyDown( KeyEvent event ) {}
    virtual void update() {}
    virtual void draw() {}

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
    bool IsTracking() { return false; }; // return m_tracking;

    // key interface TBD

private:
  	// Cached pointer to device resources.
	std::shared_ptr<DX::DeviceResources> m_deviceResources;
};

}}

#undef CINDER_APP_BASIC
#define CINDER_APP_BASIC( APP, RENDERER )

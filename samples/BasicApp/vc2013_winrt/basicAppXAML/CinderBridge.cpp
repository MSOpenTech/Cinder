// Cinder bridge to XAML
// zv new file

#include "pch.h"

#include "CinderBridge.h"

#include "..\Common\DeviceResources.h"
#include "..\Common\StepTimer.h"

//#undef min
//#undef max

using namespace basicAppXAML;

using namespace DirectX;
using namespace Windows::Foundation;

using namespace ci;
using namespace ci::app;

// cinder::app::AppBasicXAML *app;

extern cinder::app::AppImplMswRendererDx *gAIMRDx;

//AppBasicXAML::AppBasicXAML(const std::shared_ptr<DX::DeviceResources>& deviceResources) : 
//        m_deviceResources(deviceResources)

AppBasicXAML::AppBasicXAML()
{
    // create Cinder renderer for DirectX
    //  nb. Cinder will perform shader & lighting setup, and drawing via dx::
    //  initialization & device management is handled by the XAML framework

    // ren = std::shared_ptr<cinder::app::AppImplMswRendererDx>( new cinder::app::AppImplMswRendererDx( nullptr, nullptr ) );
    ren = new cinder::app::AppImplMswRendererDx( nullptr, nullptr );
    gAIMRDx = ren;

#if 0
	cinder::app::AppBasic::prepareLaunch();														
	cinder::app::AppBasic *app = new APP;														
	cinder::app::RendererRef ren(new RENDERER);													
	cinder::app::AppBasic::executeLaunch(app, ren, #APP, scPanel );								
	cinder::app::AppBasic::cleanupLaunch();														
#endif
}

AppBasicXAML::~AppBasicXAML()
{
    delete ren;
//    delete ::app; 
}

void AppBasicXAML::CreateDeviceDependentResources()
{}

void AppBasicXAML::CreateWindowSizeDependentResources()
{}

void AppBasicXAML::ReleaseDeviceDependentResources()
{}

void AppBasicXAML::Update(DX::StepTimer const& timer)
{}

void AppBasicXAML::Render()
{
    // setup for Cinder::dx
    auto context = m_deviceResources->GetD3DDeviceContext();
	auto viewport = m_deviceResources->GetScreenViewport();
    auto target = m_deviceResources->GetBackBufferRenderTargetView();
    auto stencil = m_deviceResources->GetDepthStencilView();

    ren->md3dDevice = m_deviceResources->GetD3DDevice();
    ren->mDeviceContext = context;
    ren->mMainFramebuffer = target;
    ren->mDepthStencilView = stencil;

    draw();
}



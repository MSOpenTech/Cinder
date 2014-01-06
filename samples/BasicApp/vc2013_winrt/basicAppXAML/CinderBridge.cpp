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
// extern cinder::app::AppImplMswRendererDx *gAIMRDx;

//AppBasicXAML::AppBasicXAML(const std::shared_ptr<DX::DeviceResources>& deviceResources) : 
//        m_deviceResources(deviceResources)

AppBasicXAML::AppBasicXAML() : m_tracking(false), m_pipeline_ready(false)
{
    // create Cinder renderer for DirectX
    //  nb. Cinder will perform shader & lighting setup, and drawing via dx::
    //  initialization & device management is handled by the XAML framework

    // ren = std::shared_ptr<cinder::app::AppImplMswRendererDx>( new cinder::app::AppImplMswRendererDx( nullptr, nullptr ) );
    ren = new cinder::app::AppImplMswRendererDx( nullptr, nullptr );
    // gAIMRDx = ren;


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
{
}

void AppBasicXAML::CreateWindowSizeDependentResources()
{
}

void AppBasicXAML::ReleaseDeviceDependentResources()
{}

// void AppBasicXAML::Update(DX::StepTimer const& timer)
// {}

void AppBasicXAML::Render()
{
    shareWithCinder();

    if ( !m_pipeline_ready ) {
        ren->setupPipeline();
        m_pipeline_ready = true;
    }

    float w = m_deviceResources->GetOutputSize().Width;
    float h = m_deviceResources->GetOutputSize().Height;
    ren->setupCamera( w, h );

     // calls to overloaded Cinder method
    draw();
}

void AppBasicXAML::shareWithCinder()
{
    // setup for Cinder::dx
    auto device =  m_deviceResources->GetD3DDevice();
    auto features = m_deviceResources->GetDeviceFeatureLevel();
    auto context = m_deviceResources->GetD3DDeviceContext();
	auto viewport = m_deviceResources->GetScreenViewport();
    auto target = m_deviceResources->GetBackBufferRenderTargetView();
    auto stencil = m_deviceResources->GetDepthStencilView();

    ren->md3dDevice = device;
    ren->mFeatureLevel = features;
    ren->mDeviceContext = context;
    ren->mMainFramebuffer = target;
    ren->mDepthStencilView = stencil;
}

void AppBasicXAML::TrackingUpdate(PointerEventArgs ^evtArgs)
    // float x, float y)
{
    // zv not needed?
    if ( ! m_tracking ) return;

    // std::string s( evtArgs->ToString()->Data() );
    // OutputDebugString( evtArgs->ToString()->Data() );

    auto p = evtArgs->CurrentPoint->Position;
    int ix = p.X;
    int iy = p.Y;

    /*
    float w = m_deviceResources->GetOutputSize().Width;
    float h = m_deviceResources->GetOutputSize().Height;
    int ix = w == 0.0f ? 0 : evtArgs->CurrentPoint->Position.X / w;
    int iy = h == 0.0f ? 0 : evtArgs->CurrentPoint->Position.Y / h;
    */
    //int ix = x * m_deviceResources->GetOutputSize().Width;
    //int iy = y * m_deviceResources->GetOutputSize().Height;
    //int ix = 0;
    //int iy = 0;

    // zv temp
    //
    // MouseEvent ctor:
    // WindowRef win, int aInitiator, 
    // int aX, int aY, unsigned int aModifiers, 
    // float aWheelIncrement, uint32_t aNativeModifiers
    MouseEvent e( nullptr, 0, ix, iy, cinder::app::MouseEvent::LEFT_DOWN, 0, 0 );

    // see AppImplWinRT::prepPointerEventModifiers

    mouseDrag( e );
}


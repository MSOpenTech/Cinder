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

    draw();
}



#pragma once

#include "Common\StepTimer.h"
#include "Common\DeviceResources.h"

// zv
// #include "Content\Sample3DSceneRenderer.h"
#include "CinderBridge.h"
#include "Content\SampleFpsTextRenderer.h"

// zv
using namespace Windows::UI::Core;

// Renders Direct2D and 3D content on the screen.
namespace basicAppXAML
{
	class basicAppXAMLMain : public DX::IDeviceNotify
	{
	public:
		basicAppXAMLMain(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		~basicAppXAMLMain();
		void CreateWindowSizeDependentResources();
		void StartTracking() { m_sceneRenderer->StartTracking(); }
        // zv
		// void TrackingUpdate(float x, float y) { m_sceneRenderer->TrackingUpdate(x, y); }
		void TrackingUpdate(PointerEventArgs^ e) { m_sceneRenderer->TrackingUpdate(e); }
        void StopTracking() { m_sceneRenderer->StopTracking(); }
		bool IsTracking() { return m_sceneRenderer->IsTracking(); }
		void StartRenderLoop();
		void StopRenderLoop();
		Concurrency::critical_section& GetCriticalSection() { return m_criticalSection; }

		// IDeviceNotify
		virtual void OnDeviceLost();
		virtual void OnDeviceRestored();

	private:
		void ProcessInput();
		void Update();
		bool Render();

		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		// TODO: Replace with your own content renderers.
        // zv
        // std::unique_ptr<Sample3DSceneRenderer> m_sceneRenderer;
        // std::unique_ptr<cinder::app::AppBasicXAML> m_sceneRenderer;

        // ptr to base type
        cinder::app::AppBasicXAML *m_sceneRenderer;
		std::unique_ptr<SampleFpsTextRenderer> m_fpsTextRenderer;

		Windows::Foundation::IAsyncAction^ m_renderLoopWorker;
		Concurrency::critical_section m_criticalSection;

		// Rendering loop timer.
		DX::StepTimer m_timer;
	};
}
// CaptureBasicAppWinRT
// this file was created by merging CaptureBasicApp and samples/BasicApp/basicAppDX.cpp (for XAML)

#include "cinder/Cinder.h"

#include "cinder/app/AppBasic.h"
#include "cinder/app/RendererDx.h"
#include "cinder/dx/dx.h"

#if 0
#include "cinder/app/AppNative.h"
// #include "cinder/gl/Texture.h"
#endif

#include "cinder/Capture.h"
#include "cinder/dx/dxTexture.h"

using namespace ci;
using namespace ci::app;
using namespace std;

// was:
// class CaptureBasicApp : public AppNative {
class CaptureBasicApp : public AppBasic {
public:
    void prepareSettings(Settings *settings);

    void setup();
    static void setupCompletion();

    void keyDown(KeyEvent event);

    void update();
    void draw();

    CaptureRef			mCapture;
    dx::TextureRef		mTexture;
};

void CaptureBasicApp::prepareSettings(Settings *settings)
{
#ifdef WINRT_XAML
    settings->setXaml(true);
#endif
}

void CaptureBasicApp::setup()
{
#if 1

    Capture::getDevicesAsync(true, [=]() {
        for (auto device = Capture::getDevices().begin(); device != Capture::getDevices().end(); ++device) {
            std::string s = (*device)->getName();
            console() << "Device: " << (*device)->getName() << " "
#if defined( CINDER_COCOA_TOUCH )
                << ((*device)->isFrontFacing() ? "Front" : "Rear") << "-facing"
#endif
                << std::endl;
        }
    });

    try {
        mCapture = Capture::create(640, 480 );
        mCapture->start();
    }
    catch (...) {
        console() << "Failed to initialize capture" << std::endl;
    }
#endif
}

void CaptureBasicApp::keyDown(KeyEvent event)
{
#if defined( CINDER_MAC )
    if( event.getChar() == 'f' )
        setFullScreen( ! isFullScreen() );
    else if( event.getChar() == ' ' )
        (mCapture && mCapture->isCapturing()) ? mCapture->stop() : mCapture->start();
#endif
    //    if (event.getChar() == 'T')
    if (event.getChar() == ' ') {
        if (mCapture && mCapture->isCapturing()) mCapture->stop();
    }
}

void CaptureBasicApp::update()
{
#if 1
    if (mCapture && mCapture->checkNewFrame()) {
        mTexture = dx::Texture::create(mCapture->getSurface());
    }
#endif
}

void CaptureBasicApp::draw()
{
    dx::clear(Color(0.1f, 0.5f, 0.15f));

    //	dx::clear( Color( 0.0f, 0.0f, 0.0f ) );

    dx::setMatricesWindow(getWindowWidth(), getWindowHeight());

    if (mTexture) {
        // glPushMatrix();
        dx::pushMatrices();
#if defined( CINDER_COCOA_TOUCH )
        //change iphone to landscape orientation
        dx::rotate( 90.0f );
        dx::translate( 0.0f, -getWindowWidth() );

        Rectf flippedBounds( 0.0f, 0.0f, getWindowHeight(), getWindowWidth() );
        dx::draw(mTexture, flippedBounds);
#else
        dx::draw(mTexture);
#endif
        // glPopMatrix();
        dx::popMatrices();
    }
}

// CINDER_APP_NATIVE( CaptureBasicApp, RendererGl )

#ifdef WINRT_XAML
CINDER_APP_BASIC_XAML(CaptureBasicApp, RendererDx)
#endif

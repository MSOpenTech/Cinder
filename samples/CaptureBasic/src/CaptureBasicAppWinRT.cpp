// CaptureBasicAppWinRT
// this file was created by merging CaptureBasicApp and samples/BasicApp/basicAppDX.cpp (for XAML)

#include "cinder/Cinder.h"

#include "cinder/app/AppBasic.h"
#include "cinder/app/RendererDx.h"
#include "cinder/dx/dx.h"

#if 0
#include "cinder/app/AppNative.h"
// #include "cinder/gl/Texture.h"
#include "cinder/Capture.h"
#endif

using namespace ci;
using namespace ci::app;
using namespace std;

// was:
// class CaptureBasicApp : public AppNative {
class CaptureBasicApp : public AppBasic {
  public:
    void prepareSettings(Settings *settings);

    void setup();
	void keyDown( KeyEvent event );
	
	void update();
	void draw();

	// CaptureRef			mCapture;
	// gl::TextureRef		mTexture;
};

void CaptureBasicApp::prepareSettings(Settings *settings)
{
#ifdef WINRT_XAML
    settings->setXaml(true);
#endif
}

void CaptureBasicApp::setup()
{
#if 0

	// print the devices
	for( auto device = Capture::getDevices().begin(); device != Capture::getDevices().end(); ++device ) {
		console() << "Device: " << (*device)->getName() << " "
#if defined( CINDER_COCOA_TOUCH )
					<< ( (*device)->isFrontFacing() ? "Front" : "Rear" ) << "-facing"
#endif
					<< std::endl;
	}

	try {
		mCapture = Capture::create( 640, 480 );
		mCapture->start();
	}
	catch( ... ) {
		console() << "Failed to initialize capture" << std::endl;
	}
#endif
}

void CaptureBasicApp::keyDown( KeyEvent event )
{
#if defined( CINDER_MAC )
	if( event.getChar() == 'f' )
		setFullScreen( ! isFullScreen() );
	else if( event.getChar() == ' ' )
		( mCapture && mCapture->isCapturing() ) ? mCapture->stop() : mCapture->start();
#endif
}

void CaptureBasicApp::update()
{
#if 0
	if( mCapture && mCapture->checkNewFrame() ) {
		mTexture = gl::Texture::create( mCapture->getSurface() );
	}
#endif
}

void CaptureBasicApp::draw()
{
    dx::clear(Color(0.1f, 1.0f, 0.15f));

#if 0
	gl::clear( Color( 0.0f, 0.0f, 0.0f ) );
	gl::setMatricesWindow( getWindowWidth(), getWindowHeight() );
	
	if( mTexture ) {
		glPushMatrix();
#if defined( CINDER_COCOA_TOUCH )
		//change iphone to landscape orientation
		gl::rotate( 90.0f );
		gl::translate( 0.0f, -getWindowWidth() );

		Rectf flippedBounds( 0.0f, 0.0f, getWindowHeight(), getWindowWidth() );
		gl::draw( mTexture, flippedBounds );
#else
		gl::draw( mTexture );
#endif
		glPopMatrix();
	}
#endif
}

// CINDER_APP_NATIVE( CaptureBasicApp, RendererGl )

#ifdef WINRT_XAML
CINDER_APP_BASIC_XAML(CaptureBasicApp, RendererDx)
#endif

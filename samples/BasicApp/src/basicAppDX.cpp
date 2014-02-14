#include "cinder/app/AppBasic.h"
#include "cinder/app/RendererDx.h"
#include "cinder/dx/dx.h"

#include <list>

using namespace ci;
using namespace ci::app;
using namespace std;


// We'll create a new Cinder Application by deriving from the CinderMain class

class BasicApp : public AppBasic {
public:
    void prepareSettings(Settings *settings);
    void mouseDrag(MouseEvent event);
    void keyDown( KeyEvent event );
    void draw();

	// This will maintain a list of points which we will draw line segments between
	list<Vec2f>		mPoints;
};

void BasicApp::prepareSettings(Settings *settings)
{
#ifdef WINRT_XAML
    settings->setXaml(true);
#endif
}

void BasicApp::mouseDrag( MouseEvent event )
{
    mPoints.push_back( event.getPos() );
}

void BasicApp::keyDown( KeyEvent event )
{
    char key = event.getChar();
    if (key == 'e' || key == 'E')
    {
        mPoints.clear();
    }
}

void BasicApp::draw()
{
	dx::clear(Color(0.1f, 0.1f, 0.15f));

    // zv - triangles for orientation testing
    dx::color( 0.0f, 1.0f, 0.0f );	
	dx::begin( GL_LINE_STRIP );
	dx::vertex( 50.0f, 50.0f );
	dx::vertex( 150.0f, 50.0f );
	dx::vertex( 100.0f, 100.0f );
	dx::vertex( 50.0f, 50.0f );
	dx::end();

    dx::begin( GL_LINE_STRIP );
	dx::vertex( 750.0f, 550.0f );
	dx::vertex( 850.0f, 550.0f );
	dx::vertex( 800.0f, 600.0f );
	dx::vertex( 750.0f, 550.0f );
	dx::end();

    dx::color( 1.0f, 0.5f, 0.25f );	
	dx::begin( GL_LINE_STRIP );
	for( auto pointIter = mPoints.begin(); pointIter != mPoints.end(); ++pointIter ) {
		dx::vertex( *pointIter );
	}
	dx::end();
}

// This line tells Cinder to actually create the application
#ifdef WINRT_XAML
CINDER_APP_BASIC_XAML(BasicApp, RendererDx)
#else
CINDER_APP_BASIC(BasicApp, RendererDx)
#endif

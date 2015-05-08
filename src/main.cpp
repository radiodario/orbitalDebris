#include "ofMain.h"
#include "ofApp.h"
//#include "ofGLProgrammableRenderer.h"

//========================================================================
int main( ){
//    ofAppGlutWindow window;
//    ofSetupOpenGL(&window, 1920,1080, OF_WINDOW);
//    ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
	ofSetupOpenGL(1920,1080,OF_WINDOW);			// <-------- setup the GL context
//    ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}

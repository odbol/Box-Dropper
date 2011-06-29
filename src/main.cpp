#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"
#include "ofxArgs.h"

//========================================================================
int main(int argc, char **argv){
	ofxArgs* args = new ofxArgs(argc, argv);
	
	
	
    ofAppGlutWindow window;
	
	if (args->contains("--fullscreen")) {
		ofSetupOpenGL(&window, 1024,768, OF_GAME_MODE);			// <-------- setup the GL context
	}
	else if (args->contains("--maximized")) {
		ofSetupOpenGL(&window, 1024,768, OF_FULLSCREEN);			// <-------- setup the GL context
		ofSetWindowPosition(0,0);
	}
	else {
		ofSetupOpenGL(&window, 1024,768, OF_WINDOW);			// <-------- setup the GL context
	}
	
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new testApp());

	
	delete args;
}

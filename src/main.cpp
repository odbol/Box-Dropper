#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"
#include "ofxArgs.h"

//========================================================================
int main(int argc, char **argv){
	ofxArgs* args = new ofxArgs(argc, argv);
	
	
	
    ofAppGlutWindow window;

	int xres = 1024;
	int yres = 768;
	
	if (args->contains("--xres")) {
		xres = args->getInt("--xres");
		yres = args->getInt("--yres");
	}
	
	if (args->contains("--fullscreen")) {
		ofSetupOpenGL(&window, xres,yres, OF_GAME_MODE);			// <-------- setup the GL context
	}
	else if (args->contains("--maximized")) {
		ofSetupOpenGL(&window, xres, yres, OF_WINDOW);			// <-------- setup the GL context
		
		
		ofSetWindowPosition(0,0);
		
		if (args->contains("--fullscreen")) {
			ofSetFullscreen(true);		
			ofSetWindowShape(xres, yres);	
		}
		//ofSetFullscreen(true);
	}
	else {
		ofSetupOpenGL(&window, xres,yres, OF_WINDOW);			// <-------- setup the GL context
		//ofSetWindowPosition(0,0);
	}
	
	delete args;
	
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new testApp());

}

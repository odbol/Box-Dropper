#ifndef _TEST_APP
#define _TEST_APP

//comment out to disable debug stamtents
//# define DEBUG

#include "ofMain.h"
#include "ofxFBOTexture.h"
#include "ofxShader.h"
#include "ofxVectorMath.h"

#include "lightScattering.h"


#include "BoxDropper.h"


#ifdef IS_FFGL

#include "ofxFFGLApp.h"

class testApp : public ofFFGLApp {

	Trigger<bool> eventExample; // event
	
#else

class testApp : public ofBaseApp{

#endif
	
	public:
		void setup();
		void setupRC();
	
		void update();
		void draw();
	
		void updateSpot();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
	
	
		ofxVec3f centerScene;
		
		void setupViewer(bool _transform = false);
		void drawAxis();
	
	
	//Light
	lightScattering lightScatter;
	ofxFBOTexture fboLight;
	
	GLuint dListA;
	GLuint dListB;

	
	float spotCutOff;
	float spotExponent;
	
	//position of the light ray origin
	int scatterX, scatterY;
	
	
	bool sphereEnabled;
	
	
	//boxes
private: 
	BoxDropper boxes;
	void drawBoxes();
	
	
#ifdef DEBUG
	
	int mouseMode;
#endif
};

	
#endif

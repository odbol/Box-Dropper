#ifndef _TEST_APP
#define _TEST_APP

//comment out to disable debug stamtents
//#define DEBUG

#include "ofMain.h"
#include "ofxFBOTexture.h"
#include "ofxShader.h"
#include "ofxVectorMath.h"

#include "lightScattering.h"


#include "BoxDropper.h"

#include "PaletteManager.h"

#include "ofxColor.h"


#define ROTATION_SPEED_MAX 3.0f

#define SPHERE_SIZE 20.0f

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
	
	//enables light source in bg
	bool sphereEnabled;
	//enables light source being drawn
	bool lightSphereEnabled;
	
	ofxColorf sphereColor;
	ofxColorf sphereOtherColor;
	
	float sphereHue;
	float sphereHueSpeed;
	
	bool isMonochrome;
	
	//sets openGL color in HSV space
	void pSetHSV(ofColor* color, float h, float s, float v, float a );
	void pSetColor(ofColor* color, float r, float b, float g, float a);

	//boxes
	void dropBox();
private: 
	BoxDropper boxes;
	void drawBoxes();
	
	
	int mouseMode;
	void onMouseMove(int x, int y, int mode );
	
	
	//palette
	PaletteManager palettes;
	
	
	//int windowWidth, windowHeight;  
	
	//void refreshSize();
	
};

	
#endif

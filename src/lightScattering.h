#include "ofMain.h"
#include "ofShader.h" 
#include "ofxFBOTexture.h"


class lightScattering{
	
public:
	void setup(int fboW, int fboH);
	
	void beginRender();
	void endRender();
	
	void setLightParams(float xPos, float yPos);
	void draw(float x, float y, float w, float h, bool useShader);
	
	ofShader shader;
	
	ofxFBOTexture fbo;
	
	ofPoint pos;
	float posZ;
	float zoomRadius;
	float zoomMinVal;
	float zoomMaxVal;
	
	float uniformLightX ;
	float uniformLightY ;
	float uniformExposure;
	float uniformDecay;
	float uniformDensity;
	float uniformWeight;
	
	float flickerAmount;
	
	void reset(int fboW, int fboH);
};
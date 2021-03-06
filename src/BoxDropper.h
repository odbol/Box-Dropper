#ifndef BOXDROPPER
#define BOXDROPPER

#define NUM_COLS 24
#define WALL_PADDING 5


#pragma once
//# include "ofMain.h"
#include "ofVectorMath.h"
#include "ofxBox2d.h"
#include "GlowBox.h"


//# include "ofFbo.h"
//# include "ofShader.h"
//# include "shaderBlur.h"

// ------------------------------------------------- a simple extended box2d circle
class CustomParticle : public ofxBox2dCircle {
	
public:
	CustomParticle() {
	}
	ofColor color;
	void draw() {
		float radius = getRadius();
		
		glPushMatrix();
		glTranslatef(getPosition().x, getPosition().y, 0);
		
		ofSetColor(color.r, color.g, color.b);
		ofFill();
		ofCircle(0, 0, radius);	
		
		glPopMatrix();
		
	}
};


// -------------------------------------------------

class BoxDropper  {
	
public:
	
	BoxDropper();
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void resized(int w, int h);
	
	float mass, bounce;
	
	int boxWidth, boxHeight;
	
	float px, py;
	bool							bDrawLines;
	bool							bMouseForce;
	
	ofxBox2d						box2d;			  //	the box2d world
	vector		<ofxBox2dCircle>	circles;		  //	default box2d circles
	vector		<ofxBox2dPolygon>	polygons;		  //	defalut box2d polgons
	vector		<GlowBox>		boxes;			  //	defalut box2d rects
	//vector      <ofxBox2dLine>		lines;			  //	default box2d lines (hacked)
	vector		<CustomParticle>	customParticles;  //	this is a custom particle the extends a cirlce
	
	ofxBox2dCircle					ballJoints[5];	  //	ball joints
	ofxBox2dJoint					joints[5];		  //	box2d joints
	
	//ofxBox2dLine					lineStrip;		  //	a linestrip for drawing
	
	void dropBox(int x, int y, float velocity = 0.0f);
	void dropBoxInCol(int col, float velocity = 0.0f);
	
	void killOutOfBounds();
	
	//returns number of boxes alive
	int getCount();
	
	/*
	//blur shader
	ofVideoGrabber grabber;
	bool hasCamera;
	
	shaderBlur blur;
	 */
	
	//removed ofBaseApp
	int mouseX, mouseY;
	
	
	
	//for floor
	void setFloor(bool isEnabled);
	bool isFloorEnabled;
	int floorHeight;
	
	//palettes
	ofColor strokeColor;
	void setColor(ofColor stroke);
};

#endif


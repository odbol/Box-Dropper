#ifndef GLOWBOX
#define GLOWBOX
/*
 *  GlowBox.h
 *  box_dropper
 *
 *  Created by Tyler on 6/20/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once
#include "ofMain.h"
#include "ofVectorMath.h"
#include "ofxBox2d.h"

#include <GLUT/GLUT.h>

// ------------------------------------------------- a simple extended box2d circle
class GlowBox : public ofxBox2dRect {
	
public:
	GlowBox() {
	}
	ofColor color;
	ofColor stroke;
	
	float width, height;
	
	//shape
	static bool is3D;
	static void setIs3D(bool enable3D);
	
	static float fillAlpha;
	
	void draw();
	/*
	 
	 float boxZOffset;

	void setup(b2World * b2dworld, float x, float y, float z, float w, float h, bool isFixed=false) {
		boxZOffset = z;		
		
		setup(b2dworld, x, y, w, h, isFixed);
	}
	 */
	
	
	void setup(b2World * b2dworld, float x, float y, float w, float h, bool isFixed=false);
	
};

#endif
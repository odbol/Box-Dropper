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
#include "ofxVectorMath.h"
#include "ofxBox2d.h"

// ------------------------------------------------- a simple extended box2d circle
class GlowBox : public ofxBox2dRect {
	
public:
	GlowBox() {
	}
	ofColor color;
	void draw();
};

#endif
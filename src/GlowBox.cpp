/*
 *  GlowBox.cpp
 *  box_dropper
 *
 *  Created by Tyler on 6/20/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "GlowBox.h"

void GlowBox::setup(b2World * b2dworld, float x, float y, float w, float h, bool isFixed) {
	width = w;
	height = h;
	
	ofxBox2dRect::setup(b2dworld, x, y, w, h, isFixed);
}

void GlowBox::setIs3D(bool enable3D) {
	is3D = enable3D;
	
	if (enable3D) {
		glDepthFunc(GL_LEQUAL);							// Type Of Depth Testing
		glEnable(GL_DEPTH_TEST);						// Enable Depth Testing
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			// Enable Alpha Blending (disable alpha testing)
		glEnable(GL_BLEND);	
	}
}

void GlowBox::draw() {
	if(dead) return;

	
	//rotation
	/// Get the world body origin position.
	/// @return the world position of the body's origin.
	const b2Vec2 pos = body->GetPosition();
	
	/// Get the angle in radians.
	/// @return the current world rotation angle in radians.
	float32 angle = body->GetAngle() ;
	
	
	
	ofPoint curVel = getVelocity();
	const float v = ((float)(MIN(ABS(curVel.x), 10.0f)) / 10.0f) * 55.0f + 200.0f;
	
	
	glPushMatrix();

	//glEnable(GL_CULL_FACE);
	//glScalef(1, -1, 1);           // invert Y axis so increasing Y goes back up.
	
	glTranslatef((float)pos.x*OFX_BOX2D_SCALE, (float)pos.y*OFX_BOX2D_SCALE, 0.0f);       // shift origin up to upper-left corner.
	glRotatef(angle * 180.0/PI, 0.0f, 0.0f, 1.0f);

	glColor4f(color.r, color.g, color.b, v);
	
	if (is3D) {
		glutSolidCube(width);
	}
	else {
		//glPolygonMode(GL_FRONT, GL_FILL);
		//glRectf(-width/2, -height/2, width/2, height/2);
		ofEnableAlphaBlending();
		ofSetColor(color.r, color.g, color.b, v);
		ofFill();
		ofRect(-width/2, -height/2, width, height);

		
		ofSetColor(stroke.r, stroke.g, stroke.b, v);
		ofNoFill();
		ofRect(-width/2, -height/2, width, height);

		ofDisableAlphaBlending();
		
	}
	glPopMatrix();
	
	
	
	return;
	
	
	//wow this is a pain
	b2Shape* s = body->GetShapeList();
	const b2XForm& xf = body->GetXForm();
	b2PolygonShape* poly = (b2PolygonShape*)s;
	int count = poly->GetVertexCount();
	const b2Vec2* localVertices = poly->GetVertices();
	b2Assert(count <= b2_maxPolygonVertices);
	b2Vec2 verts[b2_maxPolygonVertices];
	for(int32 i=0; i<count; ++i) {
		verts[i] = b2Mul(xf, localVertices[i]);
	}
	

	ofEnableAlphaBlending();
	ofSetColor(color.r, color.g, color.b, v);
	ofFill();
	ofBeginShape();
	for (int32 i = 0; i <count; i++) {
		ofVertex(verts[i].x*OFX_BOX2D_SCALE, verts[i].y*OFX_BOX2D_SCALE);
	}
	ofEndShape();
	
	ofSetColor(stroke.r, stroke.g, stroke.b, v);
	ofNoFill();
	ofBeginShape();
	for (int32 i = 0; i <count; i++) {
		ofVertex(verts[i].x*OFX_BOX2D_SCALE, verts[i].y*OFX_BOX2D_SCALE);
	}
	ofEndShape(true);	
	ofDisableAlphaBlending();
}

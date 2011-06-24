/*
 *  GlowBox.cpp
 *  box_dropper
 *
 *  Created by Tyler on 6/20/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "GlowBox.h"

void GlowBox::draw() {
	
	if(dead) return;
	
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
	ofSetColor(color.r, color.g, color.b, color.a);
	ofFill();
	ofBeginShape();
	for (int32 i = 0; i <count; i++) {
		ofVertex(verts[i].x*OFX_BOX2D_SCALE, verts[i].y*OFX_BOX2D_SCALE);
	}
	ofEndShape();
	
	ofSetColor(0, 255, 255);
	ofNoFill();
	ofBeginShape();
	for (int32 i = 0; i <count; i++) {
		ofVertex(verts[i].x*OFX_BOX2D_SCALE, verts[i].y*OFX_BOX2D_SCALE);
	}
	ofEndShape(true);	
	ofDisableAlphaBlending();
}

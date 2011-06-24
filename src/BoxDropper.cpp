
#include "BoxDropper.h"

//--------------------------------------------------------------
BoxDropper::BoxDropper(){
}

//--------------------------------------------------------------
void BoxDropper::setup() {
	
	mouseX = 0;
	mouseY = 0;
	
	
	ofSetVerticalSync(true);
	ofBackground(20, 20, 20);
	ofSetLogLevel(OF_LOG_NOTICE);
	
	bDrawLines  = false;
	bMouseForce = false;
	
	box2d.init();
	box2d.setGravity(0, 10);
	box2d.createFloor();
	box2d.checkBounds(true);
	box2d.setFPS(30.0);
	
	
	
	//for boxes
	mass = 50.0;
	bounce = 0.03;
	
	boxWidth = (int)((float)ofGetWidth() / NUM_COLS);
	boxHeight = 100;
	
	// the joints
	for(int i=0; i<5; i++) {
		float x = (ofGetWidth()/2) + cos(i) * 50; 
		float y = (50) + sin(i) * 50;
		ballJoints[i].setPhysics(3.0, 0.53, 0.1);
		ballJoints[i].setup(box2d.getWorld(), x, y, 10);		
	}
	// connect all the ball joints
	for(int i=1; i<5; i++) {
		joints[i].setWorld(box2d.getWorld());
		joints[i].addJoint(ballJoints[i].body, ballJoints[i-1].body, 3.0, 0.5);
		if(i == 4) {
			joints[0].setWorld(box2d.getWorld());
			joints[0].addJoint(ballJoints[4].body, ballJoints[0].body, 3.0, 0.5);
		}
	}
	
	// lets draw a simple lanscape
	ofPoint p(40, 400);
	int segs = 50;
	lineStrip.setWorld(box2d.getWorld());
	lineStrip.clear();
	for(int i=0; i<segs; i++) {
		p.x += 15;
		lineStrip.addPoint(p.x, p.y+sin(i*ofRandom(0.01, 0.5))*30);
	}
	lineStrip.createShape();
	
	
	
	//blur.setup(ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void BoxDropper::update() {
	
	
	
	box2d.update();
	
	if(bMouseForce) {
		float strength = 8.0f;
		float damping  = 0.7f;
		float minDis   = 100;
		for(int i=0; i<circles.size(); i++) {
			circles[i].addAttractionPoint(mouseX, mouseY, strength, minDis);
			circles[i].addDamping(damping, damping);
		}
		for(int i=0; i<customParticles.size(); i++) {
			customParticles[i].addAttractionPoint(mouseX, mouseY, strength, minDis);
			customParticles[i].addDamping(damping, damping);
		}
		
	}
	
}


//--------------------------------------------------------------
void BoxDropper::draw() {
	
	//blur.setBlurParams(4, (float)mouseX / 100.0);
	//blur.beginRender();
	
	
	for(int i=0; i<circles.size(); i++) {
		circles[i].draw();
	}
	for(int i=0; i<polygons.size(); i++) {
		polygons[i].draw();
	}
	for(int i=0; i<boxes.size(); i++) {
		boxes[i].draw();
	}
	for(int i=0; i<lines.size(); i++) {
		lines[i].draw();
	}
	for(int i=0; i<customParticles.size(); i++) {
		customParticles[i].draw();
	}
	
	//blur.endRender();
	//blur.draw(0, 0, ofGetWidth(), ofGetHeight(), true);
	
	
	for(int i=0; i<5; i++) ballJoints[i].draw();
	for(int i=0; i<5; i++) joints[i].draw();
	
	lineStrip.draw();
	box2d.draw();
	
	px = mouseX;
	py = mouseY;
	
	/*
	string info = "";
	info += "Press [s] to draw a line strip ["+ofToString(bDrawLines)+"]\n"; 
	info += "Press [f] to toggle Mouse Force ["+ofToString(bMouseForce)+"]\n"; 
	info += "Press [c] for circles\n";
	info += "Press [b] for blocks\n";
	info += "Press [z] for custom particle\n";
	info += "Total Bodies: "+ofToString(box2d.getBodyCount())+"\n";
	info += "Total Joints: "+ofToString(box2d.getJointCount())+"\n\n";
	info += "FPS: "+ofToString(ofGetFrameRate())+"\n";
	ofSetColor(255, 255, 255);
	ofDrawBitmapString(info, 30, 30);
	 */
}

void BoxDropper::dropBoxInCol(int col) {
	dropBox(col * boxWidth, -boxHeight);
}


void BoxDropper::dropBox(int x, int y) {

	GlowBox rect;
	rect.color.r = 1.0;
	rect.color.g = 0.0;
	rect.color.b = 0.0;
	rect.color.a = 1.0;

	rect.setPhysics(mass, bounce, 0.1);
	rect.setup(box2d.getWorld(), x, y, boxWidth, boxHeight);
	boxes.push_back(rect);
}

//--------------------------------------------------------------
void BoxDropper::keyPressed(int key) {
	
	if(key == 'c') {
		float r = ofRandom(4, 20);		// a random radius 4px - 20px
		ofxBox2dCircle circle;
		circle.setPhysics(3.0, 0.53, 0.1);
		circle.setup(box2d.getWorld(), mouseX, mouseY, r);
		circles.push_back(circle);
	}
	if(key == 'z') {
		float r = ofRandom(3, 10);		// a random radius 4px - 20px
		CustomParticle p;
		p.setPhysics(0.4, 0.53, 0.31);
		p.setup(box2d.getWorld(), mouseX, mouseY, r);
		p.color.r = ofRandom(20, 100);
		p.color.g = 0;
		p.color.b = ofRandom(150, 255);
		customParticles.push_back(p);
	}	
	if(key == 'b') {
	/*	float w = ofRandom(4, 20);	
		float h = ofRandom(4, 20);	
		ofxBox2dRect rect;
		rect.setPhysics(3.0, 0.53, 0.1);
		rect.setup(box2d.getWorld(), mouseX, mouseY, w, h);
		boxes.push_back(rect);
	 */
		dropBoxInCol(ofRandom(0, NUM_COLS));
	}
	
	if(key == 's') bDrawLines = !bDrawLines;
	if(key == 'f') bMouseForce = !bMouseForce;
	if(key == 't') ofToggleFullscreen();
}

//--------------------------------------------------------------
void BoxDropper::keyReleased(int key) {
}

//--------------------------------------------------------------
void BoxDropper::mouseMoved(int x, int y ) {
	
}

//--------------------------------------------------------------
void BoxDropper::mouseDragged(int x, int y, int button) {
	
	if(bDrawLines) {
		lineStrip.addPoint(x, y);
	}
}

//--------------------------------------------------------------
void BoxDropper::mousePressed(int x, int y, int button) {
	
	if(bDrawLines) {
		lineStrip.setWorld(box2d.getWorld());
		lineStrip.clear();
	}
	
}

//--------------------------------------------------------------
void BoxDropper::mouseReleased(int x, int y, int button){
	if(bDrawLines) lineStrip.createShape();
	
}

//--------------------------------------------------------------
void BoxDropper::resized(int w, int h){
	
}

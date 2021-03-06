/* from: plong0 @ http://forum.openframeworks.cc/index.php?topic=2729.0 which was based on http://fabiensanglard.net/lightScattering/index.php */

#include "testApp.h"

//float GlowBox::fillAlpha;

GLfloat		xrot,yrot,zrot,								// X, Y & Z Rotation
xspeed,yspeed,zspeed=0;									// X, Y & Z Spin Speed
float moveX,moveY,moveZ=0;								// X, Y & Z Position

// Rotation amounts
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

//Light values and coordinates
//Positional Light - The last value to 1.0f indictes that the light is actually present at this location
GLfloat	 lightPos[] = { 0.0f, 0.0f, 75.0f, 1.0f };
//Directional Light - The last value to 0.0f to make a light source an infinite distance away and coming
//from the direction specified by this vector
//GLfloat	 lightPos[] = { 0.0f, 0.0f, 75.0f, 0.0f };

GLfloat  specular[] = { 1.0f, 1.0f, 1.0f, 1.0f};
GLfloat  specref[] =  { 1.0f, 1.0f, 1.0f, 1.0f };

GLfloat ambientLightR, ambientLightG, ambientLightB = .6f; 
GLfloat  ambientLight[] = { .3f, .3f, .3f, 1.0f};

GLfloat  spotDir[] = { 0.0f, 0.0f, -1.0f };
GLfloat  spotLight[] = { .5f, .5f, .5f, 1.0f};

GLfloat xSpotDir, ySpotDir = 0.0f;
GLfloat zOffset = -1.0f;

// Flags for effects
#define MODE_FLAT 1
#define MODE_SMOOTH  2
#define MODE_VERYLOW 3
#define MODE_MEDIUM  4
#define MODE_VERYHIGH 5

int iShade = MODE_FLAT;
int iTess = MODE_VERYHIGH;

#define OFF_SCREEN_RENDER_RATIO 6

/*
void testApp::refreshSize() {
	int windowMode = ofGetWindowMode();  
    if(windowMode == OF_FULLSCREEN){  
        this->ofGetWidth() = ofGetScreenWidth();  
        this->ofGetHeight() = ofGetScreenHeight();  
    }  
    else if(windowMode == OF_WINDOW){  
        this->windowWidth = ofGetWidth();  
        this->windowHeight = ofGetHeight();  
    }  
}
 */

//--------------------------------------------------------------
void testApp::setup(){	
	//refreshSize();
	
	
	ofBackground(0, 0, 0);
	ofSetVerticalSync(true);

#ifndef DEBUG
	ofHideCursor();
#endif	

	
	lightScatter.setup(ofGetWidth()/OFF_SCREEN_RENDER_RATIO, ofGetHeight()/OFF_SCREEN_RENDER_RATIO);
	fboLight.allocate(ofGetWidth(), ofGetHeight());//, true);
	
	setupRC();
	
	/*
	dListA = glGenLists(1);
	glNewList(dListA,GL_COMPILE);
		glColor3ub(0, 0, 255);
		glutSolidSphere(SPHERE_SIZE, 50, 50);
	glEndList();
	
	dListB = glGenLists(1);
	glNewList(dListB,GL_COMPILE);
		glColor3ub(0, 0, 255);
		glutSolidSphere(20.0f, 50, 50);
	glEndList();
	*/
	
	sphereHueSpeed = 0.0001f;
	sphereHue = 0.0f;
	//pSetHSV(&sphereColor, 10.0f, 1.0f, 1.0f, 1.0f);
	sphereColor.set(.0, 1.0, 1.0).setMode(OF_COLOR_HSV);//.setRange(255).setMode(OF_COLOR_HSV);
	//sphereOtherColor.set(1.0, .0, 0.0).setRange(255).setMode(OF_COLOR_RGB);
	
	//myColor.set(.1, .2, .3).setRange(255).setMode(OF_COLOR_HSV).normalize().setMode(OF_COLOR_RGB);
	
	
	
	spotCutOff = 50.0f;
	spotExponent = 15.0f;
	sphereEnabled = true;

	lightSphereEnabled = false;
	
	mouseMode = 0;
#ifdef DEBUG
	lightSphereEnabled = true;
#endif		
	
	GlowBox::setIs3D(false);
	
	//init scatterer
	lightScatter.uniformExposure = 0.0156f;
	lightScatter.uniformDecay = 0.9947f;
	lightScatter.uniformDensity = 1.14f;
	lightScatter.uniformWeight = 6.6f;
	lightScatter.flickerAmount = 0.0f;
	
	//init boxes
	boxes.setup(); 
	
	
	
	//load palettes
	//boxes.setColor(palettes.getRandomPalette(int alphaVal));
	boxes.setColor(palettes.getRandomPalette(250));
	
	
	
#ifdef IS_FFGL
/*	
	//add parameters
	//rot
	addFloatParameter("rot x",&xrot,0.0,360.0);
	addFloatParameter("rot y",&yrot,0.0,360.0);
	
	//light
	addFloatParameter("scatterX",&scatterX,0.0,640.0);
	addFloatParameter("scatterY",&scatterY,0.0,480.0);
	
	addFloatParameter("exposure",&lightScatter.uniformExposure,0.0001,2.0);
	addFloatParameter("Decay",&lightScatter.uniformDecay,0.001,2.0);
	addFloatParameter("Density",&lightScatter.uniformDensity,0.001,2.0);
	addFloatParameter("Weight",&lightScatter.uniformWeight,0.001,10.0);
	

	
	// Example parameters
	
	// event parameter using a Trigger<bool> class
	// this will automatically handle value changes for us ( see draw() )
	addEventParameter("flash",&eventExample.val);
*/	
	// std::string example
	//stdString = "hello world";
	//addStringParameter("std string",&stdString);
	
	// oldschool C string with a buffer of chars
	//strcpy(cString,"hello world");
	//addCStringParameter("c string",cString);
	
#endif
}

//--------------------------------------------------------------
// This function does any needed initialization on the rendering
// context. 
void testApp::setupRC()
{
    
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
	glEnable(GL_ALPHA_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	
	glEnable(GL_DEPTH_TEST);	// Hidden surface removal
    glFrontFace(GL_CCW);		// Counter clock-wise polygons face out
    //glEnable(GL_CULL_FACE);		// Do not try to display the back sides
	
	
    // Enable lighting
    glEnable(GL_LIGHTING);
	
    // Setup and enable light 0
    // Supply a slight ambient light so the objects can be seen
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	
    // The light is composed of just a diffuse and specular components
	//glLightfv(GL_LIGHT0,GL_AMBIENT,spotLight);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,spotLight);
    glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
    glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
	
    // Specific spot effects
    // Cut off angle is 60 degrees
    glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,spotCutOff);
	//exponent propertie defines the concentration of the light
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, spotExponent);
	
	//light attenuation (default values used here : no attenuation with the distance)
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0f);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0f);
	
    // Enable this light in particular
    glEnable(GL_LIGHT0);
	
    // Enable color tracking
    glEnable(GL_COLOR_MATERIAL);
	
    // Set Material properties to follow glColor values
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	
    // All materials hereafter have full specular reflectivity
    // with a high shine
    glMaterialfv(GL_FRONT, GL_SPECULAR,specref);
    glMateriali(GL_FRONT, GL_SHININESS,30);
	
	//Emission
	//float mat_emission[] = {0.3f, 0.2f, 0.2f, 0.0f};
	//glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
	
    // Black background
    //glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
    }

//--------------------------------------------------------------
void testApp::update(){
	// Modify the ambiemt light
	// Supply a slight ambient light so the objects can be seen
	GLfloat  ambientLight[] = { ambientLightR, ambientLightG, ambientLightB, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	
	/*
	//update sphere color
	if (sphereHueSpeed != 0) {
		sphereHue = (sphereHue + sphereHueSpeed);
		if (sphereHue >= 0.02f) {
			sphereHueSpeed = -sphereHueSpeed;
			sphereHue = 0.02f;
			//sphereOtherColor.set(0,0,255);
			sphereOtherColor.set(1.0, .0, 0.0).setRange(255).setMode(OF_COLOR_RGB);
		}
		else if (sphereHue <= 0.0f) {
			sphereHueSpeed = -sphereHueSpeed;
			sphereHue = 0.0f;
			//sphereOtherColor.set(255,0,0);
			sphereOtherColor.set(1.0, .0, 0.0).setRange(255).setMode(OF_COLOR_RGB);
		}
		//pSetHSV(&sphereColor, sphereHue, 1.0f, 1.0f, 1.0f);
		
		//sphereHue = (float)mouseX / (float)ofGetWidth();
		//sphereHue = 0.01f;
		sphereColor.lerp(sphereOtherColor, ABS(sphereHueSpeed));
	}
	 */
	
	//init boxes
	boxes.update(); 
}

//--------------------------------------------------------------
void testApp::updateSpot(){
	GLfloat direction[] = {xSpotDir, ySpotDir, zOffset};
	//spot direction
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direction);
	//angle of the cone light emitted by the spot : value between 0 to 180
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, spotCutOff);
	
	//exponent propertie defines the concentration of the light
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, spotExponent);
}	


//--------------------------------------------------------------
void testApp::draw(){
	
#ifdef IS_FFGL	
	// calling isTriggered on a Trigger<> object, tells us if the internal value has changed.
	// this is handy for handling event parameters in FFGL without having to mantain two copies of a member
	if(eventExample.isTriggered())
	{
		boxes.dropBoxInCol(ofRandom(0, NUM_COLS));
	}
#endif
	
	//1 - Render offscreen with a FBO: the light source and the occluding objects, 
	//no shaders involved here. In order to save cycles, you can render to a lower 
	//resolution (factor 2 gives good results) and disable texturing/depth testing.
	fboLight.begin();//swapIn();
	ofClear(0, 0, 0, 0);
	
		glColor4f(1,1,1,1);
	
		glViewport(0,0,ofGetWidth()/OFF_SCREEN_RENDER_RATIO, ofGetHeight()/OFF_SCREEN_RENDER_RATIO);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);
	
		glPushMatrix();
		
			glScalef(1, -1, 1);           // invert Y axis so increasing Y goes down.
			glTranslatef(0, -ofGetHeight(), 0);       // shift origin up to upper-left corner.
			
			setupViewer();
			//glTranslatef(ofGetWidth()/2, ofGetHeight()/2, 500.0f);
			
			glEnable(GL_LIGHTING);
			
			if(iShade == MODE_FLAT)
				glShadeModel(GL_FLAT);
			else // 	iShade = MODE_SMOOTH;
				glShadeModel(GL_SMOOTH);	
			
			// First place the light 
			// Save the coordinate transformation
			glPushMatrix();	
			// Rotate coordinate system
			glRotatef(yRot, 0.0f, 1.0f, 0.0f);
			glRotatef(xRot, 1.0f, 0.0f, 0.0f);
			
			// Specify new position and direction in rotated coords.
			glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
			//glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,spotDir);
			updateSpot();
	
	
			if (sphereEnabled) {
				// Draw a red cone to enclose the light source
				glColor3ub(255,0,0);	
				
				// Translate origin to move the cone out to where the light
				// is positioned.
				glTranslatef(lightPos[0],lightPos[1],lightPos[2]);
				glutSolidCone(4.0f,6.0f,15,15);
				
				// Draw a smaller displaced sphere to denote the light bulb
				// Save the lighting state variables
				glPushAttrib(GL_LIGHTING_BIT);
				
				// Turn off lighting and specify a bright yellow sphere
				glDisable(GL_LIGHTING);
				glColor3ub(255,255,0);
				glutSolidSphere(3.0f, 15, 15);
				
				// Restore lighting state variables
				glPopAttrib();
			}
	
			
			// Restore coordinate transformations
			glPopMatrix();
			
			
			// Set material color and draw a sphere in the middle
			if (sphereEnabled) {
				//glColor3ub(0, 0, 255);
				glColor4f(sphereColor.getRed(), sphereColor.getGreen(), sphereColor.getBlue(), sphereColor.getAlpha());
			
				if(iTess == MODE_VERYLOW)
					glutSolidSphere(SPHERE_SIZE, 7, 7);
				else 
					if(iTess == MODE_MEDIUM)
						glutSolidSphere(SPHERE_SIZE, 15, 15);
					else //  iTess = MODE_MEDIUM;
						glutSolidSphere(SPHERE_SIZE, 30, 30);
			}
	
			//glCallList(dListA);
			/*
			glPushMatrix();	
				glTranslatef(100.0f, 0.0f, 0.0f);
				glutSolidSphere(20.0f, 50, 50);
				//glCallList(dListB);
			glPopMatrix();
			*/
	
	
	
			drawBoxes();
	
		glPopMatrix();
	

	fboLight.end();//swapOut();
	
	
	
	glViewport(0,0,ofGetWidth(),ofGetHeight());
	glEnable(GL_DEPTH_TEST);
	//2 - Clean the depth buffer, render the scene normally to the framebuffer.
	//Render the scene with no light scattering
		
	glPushMatrix();
				

		glScalef(1, -1, 1);           // invert Y axis so increasing Y goes down.
		glTranslatef(0, -ofGetHeight(), 0);       // shift origin up to upper-left corner.
	
		setupViewer();
		glDisable(GL_LIGHTING);
#ifdef DEBUG	
		drawAxis();	
#endif	
		//glTranslatef(ofGetWidth()/2, ofGetHeight()/2, 500.0f);
		
		glEnable(GL_LIGHTING);
		
		if(iShade == MODE_FLAT)
			glShadeModel(GL_FLAT);
		else // 	iShade = MODE_SMOOTH;
			glShadeModel(GL_SMOOTH);	
		
		// First place the light 
		// Save the coordinate transformation
		if (lightSphereEnabled) {
			glPushMatrix();	
				// Rotate coordinate system
				glRotatef(yRot, 0.0f, 1.0f, 0.0f);
				glRotatef(xRot, 1.0f, 0.0f, 0.0f);
			
				// Specify new position and direction in rotated coords.
				glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
				//glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,spotDir);
				updateSpot();
			
				if (sphereEnabled) {
					// Draw a red cone to enclose the light source
					glColor3ub(255,0,0);	
				
					// Translate origin to move the cone out to where the light
					// is positioned.
					glTranslatef(lightPos[0],lightPos[1],lightPos[2]);
					glutSolidCone(4.0f,6.0f,15,15);
				
					// Draw a smaller displaced sphere to denote the light bulb
					// Save the lighting state variables
					glPushAttrib(GL_LIGHTING_BIT);
				
						// Turn off lighting and specify a bright yellow sphere
						glDisable(GL_LIGHTING);
						glColor3ub(255,255,0);
						glutSolidSphere(3.0f, 15, 15);
				
					// Restore lighting state variables
					glPopAttrib();
						
				}
			
			// Restore coordinate transformations
			glPopMatrix();
			
			
			// Set material color and draw a sphere in the middle
			if (sphereEnabled) {
				//glColor4f(sphereColor.r, sphereColor.g, sphereColor.b, sphereColor.a);
				glColor4f(sphereColor.getRed(), sphereColor.getGreen(), sphereColor.getBlue(), sphereColor.getAlpha());
			
				
				if(iTess == MODE_VERYLOW)
					glutSolidSphere(SPHERE_SIZE, 7, 7);
				else 
					if(iTess == MODE_MEDIUM)
						glutSolidSphere(SPHERE_SIZE, 15, 15);
					else //  iTess = MODE_MEDIUM;
						glutSolidSphere(SPHERE_SIZE, 30, 30);
			}
		}
	
		//glCallList(dListA);
		
		/*
		glPushMatrix();	
			glTranslatef(100.0f, 0.0f, 0.0f);
			glutSolidSphere(20.0f, 50, 50);
			//glCallList(dListB);
		glPopMatrix();
		 */	
	
	
		drawBoxes();
	
	glPopMatrix();
	
	//3 - Switch to Orthogonal projection and blend the FBO with the framebuffer, 
	//activating the shader in order to generate the "God's ray" effect .
	//Paint the light scaterring effect
	lightScatter.setLightParams(scatterX, scatterY);
	lightScatter.beginRender();
		ofSetColor(255, 255, 255);
		fboLight.draw(0, 0);// /*doesn't line up right:*/, ofGetWidth(),ofGetHeight());
	lightScatter.endRender();
	
	lightScatter.draw(0, 0, ofGetWidth(),ofGetHeight(), true);

	//Debug details
#ifdef DEBUG
	glDisable(GL_LIGHTING);
	glPushMatrix();
		ofTranslate(0.0f, 0.0f, 2.0f);
		ofSetColor(255, 255, 255, 255);
		ofDrawBitmapString("Num boxes: " + ofToString(boxes.getCount()) + " FPS: " + ofToString(ofGetFrameRate()), 10, 20);
		//ofDrawBitmapString("spot pos: [" + ofToString(lightPos[0]) + ","+ ofToString(lightPos[1]) + ","+ ofToString(lightPos[2]) + "]", 10, 40);
		ofDrawBitmapString("light hue: " + ofToString(sphereHue) + " exp: [" + ofToString(lightScatter.uniformExposure) + "], decay: " + ofToString(lightScatter.uniformDecay) + ", density: " + ofToString(lightScatter.uniformDensity) + ", wight: " + ofToString(lightScatter.uniformWeight), 10, 40);
		ofDrawBitmapString(" move the light scattering source with the mouse \n"
						   "\n"
						   " arrows to move light spot \n "
						   "r : to reste spot position \n "
						   "\n"
						   " - and + : to change ambient light \n "
						   "\n"
						   " 1 and 2 : to change shade model \n "
						   "3, 4 and 5 : to change sphere tessellation \n "
						   "\n"
						   " k and i : camera x rotation \n "
						   "n and m : camera y rotation \n "
						   "j and l : camera z rotation \n "
						   "\n"
						   " z and x : move on the X \n "
						   "a and s : move on the Y \n "
						   "q and w : move on the Z \n "
						   "s: sphereEnabled \n "
						   "f: toggle floor \n "
						   "p: togglepalettes \n "
						   "shift/alt: speed \n "
						   "/: toggle mouse mode (1-5) \n "
						   , 2, 60);
	 
	glPopMatrix();
#endif
}

void testApp::drawBoxes() {
	//boxes
	glPushMatrix();
	glScalef(1, -1, 1);           // invert Y axis so increasing Y goes back up.
	glTranslatef(-ofGetWidth() / 2, -ofGetHeight() / 2, 0);       // shift origin up to upper-left corner.
	boxes.draw(); 
	glPopMatrix();
}

//--------------------------------------------------------------
void testApp::setupViewer(bool _transform) {
	centerScene.set(ofGetWidth()/2 + moveX, ofGetHeight()/2 + moveY, moveZ);
	
	glTranslatef(centerScene.x, centerScene.y, centerScene.z);
	glRotatef(xrot,1,0,0);
	glRotatef(yrot,0,1,0);
	glRotatef(zrot,0,0,1);
	
	xrot+=xspeed; yrot+=yspeed; zrot+=zspeed;
/*	
	ofNoFill();
	ofSetColor(255, 255, 255);
	glPushMatrix();
	glScalef(400.0,600.0,600.0);
	glutWireCube(1.0);
	glPopMatrix();
*/	
}

//--------------------------------------------------------------
void testApp :: drawAxis()
{
	glPushMatrix();
	glScalef( 300, 300, 300 );
	
	glBegin( GL_LINES );
	
	glColor3f(  0.7, 0, 0 );
	glVertex3f( 0, 0, 0 );
	glVertex3f( 1, 0, 0 );
	
	glColor3f(  0, 0.7, 0 );
	glVertex3f( 0, 0, 0 );
	glVertex3f( 0, 1, 0 );
	
	glColor3f(  0, 0, 0.7 );
	glVertex3f( 0, 0, 0 );
	glVertex3f( 0, 0, 1 );
	
	glEnd();
	
	glPopMatrix();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	printf("keyPressed = (%i)", key);
	//sprintf(eventString, "keyPressed = (%i)", key);
	
	const int modKey = glutGetModifiers();
	
	
	switch(key) {
		case 357: xRot-= 5.0f; break;
		case 356: yRot -= 5.0f; break;
		case 359: xRot+= 5.0f; break;
		case 358: yRot += 5.0f; break;
		case OF_KEY_BACKSPACE:
			zspeed = yspeed = xspeed = 0.0f;
			//fall through
		case 'r': 
			xRot = yRot = yrot = xrot = zrot = 0.0f; 
			break;
			

		case '+':
			ambientLightR +=.01;
			ambientLightG +=.01; 
			ambientLightB +=.01;
			break;
		case '=':
			ambientLightR +=.1;
			ambientLightG +=.1; 
			ambientLightB +=.1;
			break;
		
		case '_':
			ambientLightR -=.01;
			ambientLightG -=.01; 
			ambientLightB -=.01;
			break;
		case '-':
			ambientLightR -=.1;
			ambientLightG -=.1; 
			ambientLightB -=.1;
			break;

		/*	
		case '1':
            iShade = MODE_FLAT;
            break;
			
        case '2':
            iShade = MODE_SMOOTH;
            break;
			
        case '3':
            iTess = MODE_VERYLOW;
            break;
			
        case '4':
            iTess = MODE_MEDIUM;
            break;
			
        case '5':
            iTess = MODE_VERYHIGH;
            break;
		 */
			
		case OF_KEY_RETURN: ofToggleFullscreen(); break;
			
		case ' ':
		case 'B':
		case 'b': {
			float vel = ofRandom(0.1f, 2.0f);
			if (modKey & GLUT_ACTIVE_SHIFT) {  
				vel += 10.0f;
			}
			if (modKey & GLUT_ACTIVE_ALT) {  
				vel += 30.0f;
			}  
			if (modKey & GLUT_ACTIVE_CTRL) {  
				vel += 50.0f;
			}  
		
			dropBox(vel); 
			break;	
		}	
		case 'j': zspeed+=0.01f; break;
		case 'l': zspeed-=0.01f; break;
		case 'k': xspeed+=0.01f; break;
		case 'i': xspeed-=0.01f; break;
		case 'n': yspeed+=0.01f; break;
		case 'm': yspeed-=0.01f; break;
			
		case 'q': moveZ+=10.0f; break;
		case 'w': moveZ-=10.0f; break;
		case 'a': moveY+=10.0f; break;
		//case 's': moveY-=10.0f; break;
		case 'z': moveX+=10.0f; break;
		case 'x': moveX-=10.0f; break;
			
		//xSpotDir, ySpotDir, zOffset
		case ',': xSpotDir+=.1f; break;
		case '.': xSpotDir-=.1f; break;
		case '<': zOffset += 1.0f; break;
		case '>': zOffset -= 1.0f; break;
			
		case 'c':
			//spotCutOff+=10; 
			if (GlowBox::fillAlpha > 0)
				GlowBox::fillAlpha = 0.0f;
			else {
				GlowBox::fillAlpha = 220.0f;
			}

			break;
		case 's': sphereEnabled = !sphereEnabled; break;
		case 'f': boxes.setFloor(!boxes.isFloorEnabled); break;
		case 'p': palettes.togglePalette(); break;
			

		case '/':	
			mouseMode = (mouseMode + 1) % 6;
			break;

		case '0':
			mouseMode = -1;
			break;			
		case '1':
			mouseMode = 0;
			break;
		case '2':
			mouseMode = 1;
			break;
		case '3':
			mouseMode = 2;	
			break;
		case '4':
			mouseMode = 3;	
			break;	
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	//spotCutOff = ((float)mouseX / (float)ofGetWidth()) * 180.0f;
	//spotExponent = ((float)mouseY / (float)ofGetHeight()) * 50.0f;
	onMouseMove(x, y, mouseMode);
	
}

void testApp::onMouseMove(int x, int y, int mode ){
	switch (mode) {
			
		case 1:	
			sphereColor.set(MAX(0.0f, MIN(0.9999f, ((float)mouseY / (float)ofGetHeight()))), 1.0f, 1.0f);//.setRange(255).setMode(OF_COLOR_HSV);
			break;
		case 2:
			yspeed = ((float)mouseX / (float)ofGetWidth()) * ROTATION_SPEED_MAX - ROTATION_SPEED_MAX / 2.0f;
			break;
			/*
		case 2:
			lightPos[0] = ((float)mouseX / (float)ofGetWidth()) * ofGetWidth() - ofGetWidth() / 2.0f;
			lightPos[1] = ((float)mouseY / (float)ofGetHeight()) * ofGetHeight() - ofGetHeight() / 2.0f;
			break;		
		case 3:
			lightPos[1] = ((float)mouseY / (float)ofGetHeight()) * ofGetHeight() - ofGetHeight() / 2.0f;
			lightPos[2] = ((float)mouseX / (float)ofGetWidth()) *  ofGetWidth() - ofGetWidth() / 2.0f;
			break;		
				

		case 2:
			xspeed = ((float)mouseY / (float)ofGetHeight()) * ROTATION_SPEED_MAX - ROTATION_SPEED_MAX / 2.0f;
			break;	*/	
		case 3:
			xspeed = ((float)mouseX / (float)ofGetWidth()) * ROTATION_SPEED_MAX - ROTATION_SPEED_MAX / 2.0f;
			break;	
		
			
		case 4:
			lightScatter.uniformExposure = ((float)mouseX / (float)ofGetWidth() * 10.0f) * 0.0034f;
			lightScatter.uniformDecay = ((float)mouseY / (float)ofGetHeight() * 2.0f) * 1.0f;
			break;
		case 5:
			lightScatter.uniformDensity = ((float)mouseX / (float)ofGetWidth() * 10.0f) * 0.84f;
			lightScatter.uniformWeight = ((float)mouseY / (float)ofGetHeight() * 10.0f) * 5.65f;
			break;	
		case 0:
			scatterX = mouseX;
			scatterY = ofGetHeight() - mouseY;

			break;
	}

//	scatterX = mouseX;
//	scatterY = ofGetHeight() - mouseY;
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	onMouseMove(x, y, mouseMode >= 0 ? button : mouseMode);
}

void testApp::dropBox(float velocity) {
	//load palettes
	if (!isMonochrome) {
		boxes.setColor(palettes.getRandomPalette(250));
	}
	
	
	boxes.dropBoxInCol(ofRandom(0, NUM_COLS), velocity);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
	dropBox();
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	//refreshSize();
	
	//ofSetWindowPosition(0,-100);
	
	lightScatter.reset(ofGetWidth()/OFF_SCREEN_RENDER_RATIO, ofGetHeight()/OFF_SCREEN_RENDER_RATIO);
	
	
	//fboLight.clean();
	fboLight.allocate(ofGetWidth(), ofGetHeight());//, true);
	
}



void testApp::pSetHSV(ofColor* color, float h, float s, float v, float a ) {  
	// H [0, 360] S, V and A [0.0, 1.0].  
	int i = (int)floor(h/60.0f) % 6;  
	float f = h/60.0f - floor(h/60.0f);  
	float p = v * (float)(1 - s);  
	float q = v * (float)(1 - s * f);  
	float t = v * (float)(1 - (1 - f) * s);  
	
	switch (i) {     
		case 0: pSetColor(color, v, t, p, a);  
			break;  
		case 1: pSetColor(color, q, v, p, a);  
			break;  
		case 2: pSetColor(color, p, v, t, a);  
			break;  
		case 3: pSetColor(color, p, q, v, a);  
			break;  
		case 4: pSetColor(color, t, p, v, a);  
			break;  
		case 5: pSetColor(color, v, p, q, a);  
	}  
}

void testApp::pSetColor(ofColor* color, float r, float b, float g, float a) {
	color->r = r * 255.0f;
	color->b = b * 255.0f;
	color->g = g * 255.0f;
	color->a = a * 255.0f;
}


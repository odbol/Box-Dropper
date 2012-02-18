#ifndef PALETTE_MANAGER
#define PALETTE_MANAGER

#include "ofMain.h"

//# include "ofDirectory.h"

//using namespace std;

/*
 *  PaletteManager.h
 *  light_scatterer
 *
 *  Created by Tyler on 6/28/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */


// ------------------------------------------------- a simple extended box2d circle
class PaletteManager  {
	
public:
	
#define NUM_PALETTES 3
	static const int palette[];
	

	static const char* palettes[];
	
	
	PaletteManager() {
		paletteIdx = 0;
		
				
		//palettes = {"file1", "file2"}
	}

	
	ofColor getRandomPalette(int alphaVal);
	void togglePalette(void); 

	
private:
	
	ofImage img;
	
	int paletteIdx;
};

#endif
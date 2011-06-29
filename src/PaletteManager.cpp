/*
 *  PaletteManager.cpp
 *  light_scatterer
 *
 *  Created by Tyler on 6/28/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "PaletteManager.h"

const int PaletteManager::palette[] = {0,1,2};
const char* PaletteManager::palettes[] = {"data/palettes/mood.jpg", "data/palettes/sowervangogh.jpg"};


ofColor PaletteManager::getRandomPalette(int alphaVal) {
	ofColor newColor;
	
	if (true) {// palletes != null) {
		int bytes_per_pixel = img.bpp / 8;
		if (img.type != OF_IMAGE_COLOR || bytes_per_pixel != 3) {
			togglePalette();
		}
		
		//if (&img != NULL) {
			if (img.type == OF_IMAGE_COLOR && bytes_per_pixel == 3) {
				// This function only works on RGB images.
					
				int w = img.width;
				int h = img.height;
				
				//int pixIdx = (int) random(0,bg.pixels.length); //(int)loc.x + (int)loc.y * bg.width;
				
				//if (pixIdx >= bg.pixels.length || pixIdx < 0)
				//  pixIdx = 0;
				//println(bg.pixels.length + " > " + pixIdx);
				int pxX = (int) ofRandom(0, w * h);
				//int pxY = (int) random(0, h);
				
				
				
				unsigned char* im_pixels = img.getPixels(); // These are the pixel data of im,
				//unsigned char* p = new unsigned char[w*h*bytes_per_pixel]; // and here's where we keep our processed pixel data.

					int base = pxX * bytes_per_pixel;
					
					// Assuming the pixels come in RGB order, here we grab the intensity
					// (a value in the range [0,255]) of each color channel of the image.
					unsigned char r = im_pixels[base + 0];
					unsigned char g = im_pixels[base + 1];
					unsigned char b = im_pixels[base + 2];
					
					
				
					//p[base + 0] = 255 - r; // Invert R.
					//p[base + 1] = 255 - g; // Invert G.
					//p[base + 2] = 255 - b; // Invert B.

				

				newColor.r = (float)r;
				newColor.g = (float)g;
				newColor.b = (float)b;
				newColor.a = alphaVal;
			}
		//}
	}

	return newColor;

	//return color(ofRandom(0,255),ofRandom(0,255),ofRandom(0,255),alphaVal); 
}


void PaletteManager::togglePalette() {
	//if (img != NULL)
	//	delete img;
	
	ofxDirList   DIR;
	DIR.setVerbose(false);
    int nImages = DIR.listDir("palettes");
 	
	printf("\nloading palette %s\n", DIR.getPath(paletteIdx).c_str());
	img.loadImage(DIR.getPath(paletteIdx));	
	
		
	//img.loadImage(palettes[paletteIdx]);

	
	paletteIdx = (paletteIdx + 1) % nImages;
}


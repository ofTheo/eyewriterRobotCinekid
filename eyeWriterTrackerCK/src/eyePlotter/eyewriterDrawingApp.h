/*
 *  eyewriterDrawingApp.h
 *  CinekidRobot
 *
 *  Created by theo on 14/10/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

//#include "ofxDirList.h"
//#include "ofxVectorMath.h"
//#include "ofxNetwork.h"
//#include "ofxOsc.h"
//#include "ofxThread.h"
//#include "ofxXmlSettings.h"

#include "drawingScene.h"
#include "eyeCursor.h"

class eyewriterDrawingApp{

	public:

		void setup(float x, float y, float w, float h);
		void update();
		void updatePoint(float x, float y, float smoothAmnt);
		void draw();
			
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void keyPressed(int key);
							
		drawingScene drawScene;
		ofRectangle bounds;
		eyeCursor cursor;
		
};


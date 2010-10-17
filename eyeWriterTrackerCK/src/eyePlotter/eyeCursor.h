/*
 *  eyeCursor.h
 *  CinekidRobot
 *
 *  Created by theo on 15/10/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#include "ofMain.h"

class eyeCursor{
	public:
				
		void update(float x, float y, float smoothAmnt);
		void draw();
		
		ofPoint currentPoint;
		
};
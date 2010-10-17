/*
 *  eyeCursor.cpp
 *  CinekidRobot
 *
 *  Created by theo on 15/10/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "eyeCursor.h"


void eyeCursor::update(float x, float y, float smoothSpeed){
	
	float rateAdj = 60.0f / ofClamp( ofGetFrameRate(), 2, 900 ); 
	float fpsPct  = ofClamp( smoothSpeed * rateAdj, 0.01, 0.99);
		
	currentPoint.x  *= (1.0-fpsPct);
	currentPoint.x  += fpsPct * (float)x;
	
	currentPoint.y  *= (1.0-fpsPct);
	currentPoint.y += fpsPct * (float)y;		
		
}

void eyeCursor::draw(){
	ofPushStyle();
		ofNoFill();
		ofCircle(currentPoint.x, currentPoint.y, 8);
	ofPopStyle();
}
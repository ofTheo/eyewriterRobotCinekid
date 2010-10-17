/*
 *  eyewriterDrawingApp.cpp
 *  CinekidRobot
 *
 *  Created by theo on 14/10/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "eyewriterDrawingApp.h"

void eyewriterDrawingApp::setup(float x, float y, float w, float h){
	bounds = ofRectangle(x, y, w, h);
	drawScene.setup(x, y, w, h);
}

void eyewriterDrawingApp::update(float x, float y, float smoothAmnt){

	if( x < bounds.x ) x = bounds.x;
	if( y < bounds.y ) y = bounds.y;
	if( x > bounds.x + bounds.width )	x = bounds.x + bounds.width;
	if( y > bounds.y + bounds.height )	y = bounds.y + bounds.height;
	
	cursor.update(x, y, smoothAmnt);
	drawScene.update(cursor.currentPoint.x, cursor.currentPoint.y);	
	
}

void eyewriterDrawingApp::draw(){
	ofPushStyle();
	
	drawScene.draw();
	
	ofSetColor(50, 200, 60);
	cursor.draw();

	ofPopStyle();
}
		

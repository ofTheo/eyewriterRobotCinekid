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

void eyewriterDrawingApp::update(){
	drawScene.update();
	
}

void eyewriterDrawingApp::updatePoint(float x, float y, float smoothAmnt){

	if( x < bounds.x ) x = bounds.x;
	if( y < bounds.y ) y = bounds.y;
	if( x > bounds.x + bounds.width )	x = bounds.x + bounds.width;
	if( y > bounds.y + bounds.height )	y = bounds.y + bounds.height;
		
	if( drawScene.bOffsetCheck ){
		cursor.update(x, y, smoothAmnt);
	}else{
		cursor.update(x+drawScene.offset.x, y+drawScene.offset.y, smoothAmnt);	
	}

	drawScene.updatePoint(cursor.currentPoint.x, cursor.currentPoint.y);	

}

void eyewriterDrawingApp::draw(){
	ofPushStyle();
	
	drawScene.draw();
	
	ofSetColor(50, 200, 60);
	cursor.draw();

	ofPopStyle();
}

void eyewriterDrawingApp::mouseDragged(int x, int y, int button){
	drawScene.mouseDragged(x, y, button);
}

void eyewriterDrawingApp::mousePressed(int x, int y, int button){
	drawScene.mousePressed(x, y, button);
}

void eyewriterDrawingApp::mouseReleased(int x, int y, int button){
	drawScene.mouseReleased(x, y, button);
}

void eyewriterDrawingApp::keyPressed(int key){
	drawScene.keyPressed(key);
}


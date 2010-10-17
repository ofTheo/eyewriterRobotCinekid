/*
 *  strokeManager.cpp
 *  CinekidRobot
 *
 *  Created by theo on 15/10/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "strokeManager.h"

void strokeManager::clear(){
	group.clear();
	sendEvent("clear");				
}

void strokeManager::addPoint(float x, float y){

	if( group.size() == 0 ){
		group.addGroup();
		sendEvent("madeNewGroup");			
	}
	
	group.back().addPoint(x, y, ofGetElapsedTimef(), 1.0);
	sendEvent("addPoint");				
}

void strokeManager::undoLastPoint(){
	if( group.size() > 0 ){
		if( group.back().hasPoints() ){
			stroke & s = group.back().strokes.back();
			s.undo();
		}
		sendEvent("undo");	
	}
}

void strokeManager::newStroke(){
	if( group.back().strokes.size() && group.back().getLastStrokeNumPoints() <= 1 ){
		group.back().strokes.back().pts.clear();
	}else{
		//otherwise make a new stroke.
		group.back().begin();
	}
	
	sendEvent("newStroke");	
}

void strokeManager::sendEvent(string eventType){
	strokeEvent tmpEvent;
	tmpEvent.strokeAction = eventType;
	ofNotifyEvent(actionEvent, tmpEvent, this);
}

void strokeManager::newShape(){
	if( group.back().getTotalNumPoints() > 1 ){

		group.addGroup();
		group.back().begin();
		group.back().style = groupStyle(group.size());
		
		sendEvent("madeNewShape");

	}else{
		group.back().clear();
	}

	sendEvent("newShape");
		
}

void strokeManager::drawAllShapes(){
	
	if( group.size() ){
		for(int i = 0; i < group.size(); i++){
			group[i].draw();
		}
	}
}

void strokeManager::drawCurrentShape(){
	if( group.size() ){
		group.back().draw();
	}
}

void strokeManager::drawGuideLine(float tx, float ty){

	if( group.size() && group.back().hasPoints() ){

		if( !group.back().newStrokeOnNext ){
			ofPoint pt = group.back().getLastPoint();

			glLineStipple(1, 0x3F07 );
			glEnable( GL_LINE_STIPPLE );
			ofLine(tx, ty, pt.x, pt.y);
			glDisable(GL_LINE_STIPPLE);
		}
	}

}



